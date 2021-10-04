#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>
#include <debug/debug.h>

MemoryAllocator::MemoryAllocator( uintptr_t heapIncrement)
: _heapIncrement(heapIncrement)
{

}

void MemoryAllocator::AddFree(item_t* item)
{
    INFO("Add " << item->base << ", " << item->bytes << "bytes.");
    if(_freeCount == 0)
    {
        _free = item;
        _free->next = nullptr;
        _freeCount++;
        return;
    }
    else
    {
        item_t* freeItem = _free;
        item_t* parent = nullptr;
        while(freeItem != nullptr)
        {
            if(freeItem->bytes >= item->bytes)
            {
                item->next = freeItem;
                if(parent != nullptr) parent->next = item;
                _freeCount++;

                if(freeItem == _free) _free = item;
                return;
            }
            parent = freeItem;
            freeItem = freeItem->next;
        }

        //  end of list and no insertion yet
        parent->next = item;
        item->next = nullptr;
        _freeCount++;
    }

}

void* MemoryAllocator::Allocate(size_t bytes)
{
    if(bytes % sizeof(uintptr_t)) bytes+= sizeof(uintptr_t) - (bytes % sizeof(uintptr_t));

    item_t* freeItem = _free;
    while(freeItem != nullptr)
    {
        INFO("Bytes: " << freeItem->bytes);
        if(freeItem->bytes >= bytes)
        {
            if(freeItem->bytes >= (bytes + 0x100))
            {
                size_t oldBytes = freeItem->bytes;

                freeItem->bytes = bytes;

                item_t* newItem = (item_t*)(((uintptr_t)freeItem) + freeItem->bytes);
                newItem->base = ((uint64_t)newItem + sizeof(item_t));
                newItem->bytes = oldBytes - sizeof(item_t);
                
                AddFree(newItem);
            }
            
            RemoveFree(freeItem);
            return (void*)freeItem->base;
        }
        INFO("NEXT");
        freeItem = freeItem->next;
    }

    ExtendHeap(bytes + sizeof(item_t));
    return Allocate(bytes);
}

void MemoryAllocator::Free(void* ptr)
{
    AddFree((item_t*)ptr);
}

void MemoryAllocator::ExtendHeap(size_t bytes)
{
    FATAL("Unable to extend heap by " << bytes);
}

void MemoryAllocator::Initialise( requestHeapBytes allocator )
{
    //  set up the free pages linked list and get some initial allocation
    //  space from the kernel.
    _requestHeapBytes = allocator;

    if(_heapIncrement < 0x1000) _heapIncrement = 0x1000;
    if(_heapIncrement % 0x1000) _heapIncrement+= (0x1000 - (_heapIncrement % 0x1000));

    _heapStart = _requestHeapBytes(0);
    if(_heapStart == 0) FATAL("Unable to set start of kernel heap.");

    _heapTop = _requestHeapBytes(KERNEL_HEAP_INCREMENT);

    _freeCount = 0;

    //  place our first free list structure at the start of the heap
    item_t* item = (item_t*)_heapStart;
    item->base = ((uint64_t)_heapStart) + sizeof(item_t);
    item->bytes = HeapSize() - sizeof(item_t);

    AddFree(item);
}

void MemoryAllocator::RemoveFree(item_t* item)
{
    if(item == _free)
        _free = item->next;
    else
    {
        item_t* freeItem = _free->next;
        item_t* parent = _free;
        while(freeItem != nullptr)
        {
            if(freeItem == item)
            {
                parent->next = item->next;
                break;
            }
            parent = freeItem;
            freeItem = freeItem->next;
        }
    }

    _freeCount--;
}
