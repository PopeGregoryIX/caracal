#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>
#include <debug/debug.h>

MemoryAllocator::MemoryAllocator( uintptr_t heapIncrement)
: _heapIncrement(heapIncrement)
{

}

void* MemoryAllocator::Allocate(size_t bytes)
{
    (void)bytes;
    FATAL("Allocate memory in the kernel");
    return nullptr;
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

    //  place our first free list structure at the start of the heap
    listItem_t* item = (listItem_t*)_heapStart;
    item->data.base = ((uint64_t)_heapStart) + sizeof(listItem_t);
    item->data.bytes = HeapSize() - ((uint64_t)_heapStart) + sizeof(listItem_t);

    _free.Add(*item);
}