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
        uintptr_t _heapMax;
        uintptr_t _heapSize;

        requestHeapBytes _heapManager;
    public:
        MemoryAllocator(uintptr_t heapStart, uintptr_t heapMax);

        inline bool IsAllocated(uintptr_t address)
        { return (address >= _heapStart && address < (_heapStart + _heapSize)); }

        inline void SetHeapManager(requestHeapBytes heapManager) { _heapManager = heapManager; }
};

#endif