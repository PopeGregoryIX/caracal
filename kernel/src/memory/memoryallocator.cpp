#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>
#include <debug/debug.h>

MemoryAllocator::MemoryAllocator( uintptr_t heapIncrement)
: _heapIncrement(heapIncrement), _free(nullptr)
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
        FATAL("Free list contains " << (uintptr_t)_free);
}

void* MemoryAllocator::Allocate(size_t bytes)
{
    (void)bytes;
    return (void*)(_free->base);
}

void MemoryAllocator::Free(void* ptr)
{
    (void)ptr;
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
    INFO("New heap will start at" << _heapStart);
    if(_heapStart == 0) FATAL("Unable to set start of kernel heap.");

    _heapTop = _requestHeapBytes(KERNEL_HEAP_INCREMENT);

    //  place our first free list structure at the start of the heap
    _free = (item_t*)_heapStart;
    _free->base = ((uint64_t)_heapStart) + sizeof(item_t);
    _free->bytes = HeapSize() - sizeof(item_t);
}

void MemoryAllocator::RemoveFree(item_t* item)
{
    (void)item;
}
