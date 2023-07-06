#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>
#include <debug/debug.h>

MemoryAllocator::MemoryAllocator( uintptr_t heapIncrement):
_heapIncrement(heapIncrement),
_heapStart(0),
_heapTop(0),
_free(nullptr),
_used(nullptr),
_requestHeapBytes(nullptr)
{

}

void MemoryAllocator::AddFree(item_t* item)
{
    if(_free == nullptr)
    {
        item->next = nullptr;
        _free = item;
    }
    else
    {
        item_t* current = _free;
        while(current != nullptr)
        {
            if(item->bytes > current->bytes)
            {
                item->next = current->next;
                current-> next = item;
                return;
            }
            current = current->next;
        }
    }
}

void MemoryAllocator::AddUsed(MemoryAllocator::item_t *item)
{
    if(_used == nullptr)
    {
        item->next = nullptr;
        _used = item;
    }
    else
    {
        item_t* current = _used;
        while(current != nullptr)
        {
            if(item->bytes > current->bytes)
            {
                item->next = current->next;
                current-> next = item;
                return;
            }
            current = current->next;
        }
    }
}

void* MemoryAllocator::Allocate(size_t bytes)
{
    //  align the allocation
    if((bytes % sizeof(uintptr_t)) != 0) bytes+= (sizeof(uintptr_t) - (bytes % sizeof(uintptr_t)));

    item_t* item = _free;
    while(item != nullptr)
    {
        if(item->bytes >= bytes)
        {
            _lock.Acquire();
            RemoveFree(item);

            //  See if this region is large enough to split
            if((item->bytes - bytes) > KERNEL_HEAP_MINSPLIT)
            {
                item_t* newItem = (item_t*)(item->base + bytes);
                newItem->base = ((uintptr_t)newItem) + sizeof(item_t);
                newItem->bytes = ((item->bytes - bytes)) - sizeof(item_t);

                item->bytes = bytes;
                AddFree(newItem);
            }

            AddUsed(item);
            _lock.Release();
            return (void*)(item->base);
        }
        item = item->next;
    }

    //  the allocation failed - see if we can extend the heap
    ExtendHeap(bytes);
    return Allocate(bytes);
}

void MemoryAllocator::ExtendHeap(uintptr_t bytes)
{
    _lock.Acquire();
    bytes+= sizeof(item_t);
    if(bytes < KERNEL_HEAP_INCREMENT) bytes = KERNEL_HEAP_INCREMENT;

    uintptr_t oldHeapTop = _heapTop;
    _heapTop = _requestHeapBytes(bytes);
    uintptr_t additionalSize = _heapTop - oldHeapTop;
    if(additionalSize < bytes) FATAL("Unable to extend heap by " << bytes);

    item_t* newItem = (item_t*)oldHeapTop;
    newItem->base = ((uint64_t)oldHeapTop) + sizeof(item_t);
    newItem->bytes = additionalSize - sizeof(item_t);
    AddFree(newItem);
    _lock.Release();
}

void MemoryAllocator::Free(void* ptr)
{
    (void)ptr;
}

void MemoryAllocator::Initialise( requestHeapBytes allocator )
{
    if(KERNEL_HEAP_MINSPLIT < (sizeof(item_t) * 2))
        FATAL("KERNEL_HEAP_MINSPLIT must be at least " << (sizeof(item_t) * 2) <<" bytes to enable sensible heap management.");

    //  set up the free pages linked list and get some initial allocation
    //  space from the kernel.
    _requestHeapBytes = allocator;

    if(_heapIncrement < 0x1000) _heapIncrement = 0x1000;
    if(_heapIncrement % 0x1000) _heapIncrement+= (0x1000 - (_heapIncrement % 0x1000));

    _heapStart = _requestHeapBytes(0);  //  DO NOT USE EXTENDHEAP!
    //INFO("New heap will start at" << _heapStart);
    if(_heapStart == 0) FATAL("Unable to set start of kernel heap.");

    _heapTop = _requestHeapBytes(KERNEL_HEAP_INCREMENT);

    //  place our first free list structure at the start of the heap
    _free = (item_t*)_heapStart;
    _free->base = ((uint64_t)_heapStart) + sizeof(item_t);
    _free->bytes = HeapSize() - sizeof(item_t);
}

void MemoryAllocator::RemoveFree(item_t* item)
{
    item_t* parent = item->Parent(_free, item);

    if (parent == nullptr)
        //  this was the first item in the list
        _free = item->next;
    else
        parent->next = item->next;

    item->next = nullptr;
}

void MemoryAllocator::RemoveUsed(item_t* item)
{
    item_t* parent = item->Parent(_used, item);

    if (parent == nullptr)
        //  this was the first item in the list
        _used = item->next;
    else
        parent->next = item->next;

    item->next = nullptr;
}
