#ifndef __MEMORYALLOCATOR__H_
#define __MEMORYALLOCATOR__H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>

typedef uintptr_t (*requestHeapBytes)(intptr_t bytes);

class MemoryAllocator
{
    private:
        uintptr_t _heapStart;
        uintptr_t _heapIncrement;
        uintptr_t _heapTop;

        requestHeapBytes _requestHeapBytes;

        inline uintptr_t HeapSize( void ) { return _heapTop - _heapStart; } 

        typedef struct MemoryAllocatorArea
        {
            uintptr_t base;
            size_t bytes;

            MemoryAllocatorArea* next;
        } item_t;

        item_t* _free;

        void ExtendHeap(size_t bytes);
    public:
        // linked list management
        void AddFree( item_t* item);
        void RemoveFree( item_t* item);

    public:
        MemoryAllocator( uintptr_t heapIncrement );

        void* Allocate(size_t bytes);

        void Free(void* ptr);

        void Initialise( requestHeapBytes heapManager );

        inline bool IsAllocated(uintptr_t address)
        { return (address >= _heapStart && address < _heapTop); }
};

#endif