#ifndef __MEMORYALLOCATOR__H_
#define __MEMORYALLOCATOR__H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <support/templates/ordereddoublelinkedlist.h>

typedef uintptr_t (*requestHeapBytes)(intptr_t bytes);

class MemoryAllocator
{
    private:
        uintptr_t _heapStart;
        uintptr_t _heapIncrement;
        uintptr_t _heapTop;

        requestHeapBytes _requestHeapBytes;

        inline uintptr_t HeapSize( void ) { return _heapTop - _heapStart; } 

        struct MemoryAllocatorArea
        {
            uintptr_t base;
            size_t bytes;
        };

        typedef OrderedDoubleLinkedList<MemoryAllocator::MemoryAllocatorArea>::doubleLinkedListItem listItem_t;

        OrderedDoubleLinkedList<MemoryAllocator::MemoryAllocatorArea> _free;
    public:
        MemoryAllocator( uintptr_t heapIncrement );

        void* Allocate(size_t bytes);

        void Initialise( requestHeapBytes heapManager );

        inline bool IsAllocated(uintptr_t address)
        { return (address >= _heapStart && address < _heapTop); }
};

#endif