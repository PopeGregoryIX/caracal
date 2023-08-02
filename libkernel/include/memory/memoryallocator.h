#ifndef __MEMORYALLOCATOR__H_
#define __MEMORYALLOCATOR__H_

#include <stdint.h>
#include <stddef.h>
#include <memory/spinlock.h>
#include <debug.h>
#include <memory/heapmanager.h>

/**
 * @brief Allocate memory in a given area. Equivalent to malloc.
*/
class MemoryAllocator
{
    private:
        typedef struct MemoryAllocatorArea
        {
            uintptr_t base;
            size_t bytes;

            MemoryAllocatorArea* next;

            MemoryAllocatorArea* Parent(MemoryAllocatorArea* first, MemoryAllocatorArea* item)
            {
                MemoryAllocatorArea* currentItem = first;
                MemoryAllocatorArea* parentItem = nullptr;
                while(currentItem != nullptr)
                {
                    if(currentItem == item) return parentItem;
                    parentItem = currentItem;
                    currentItem = currentItem-> next;
                }

                FATAL("Attempt to search for a MemoryAllocatorArea which does not exist.");
                return nullptr;
            }
        } item_t;
    private:
        uintptr_t _heapIncrement;
        uintptr_t _heapStart;
        uintptr_t _heapTop;
        uintptr_t _memoryType;
        item_t* _free;
        item_t* _used;
        arch::HeapManager& _heapManager;
        Spinlock _lock;
        
        inline uintptr_t HeapSize( void ) { return _heapTop - _heapStart; }

        void ExtendHeap(uintptr_t bytes);
    private:
        // linked list management - do not use one of the templates - we need more control
        void AddFree( item_t* item);
        void RemoveFree( item_t* item);

        void AddUsed( item_t* item);
        void RemoveUsed( item_t* item);
    public:
        MemoryAllocator( arch::HeapManager& heapmanager, uintptr_t heapIncrement, uintptr_t memoryType );

        void* Allocate(size_t bytes);

        void Free(void* ptr);

        inline uintptr_t GetMemoryType( void ) { return _memoryType; }

        void Initialise( void );

        inline bool IsAllocated(uintptr_t address)
        { return (address >= _heapStart && address < _heapTop); }
};

#endif