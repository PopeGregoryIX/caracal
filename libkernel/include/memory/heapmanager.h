#ifndef __HEAPMANAGER_H_
#define __HEAPMANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include <memory/pageframeallocator.h>
#include <debug.h>
#include <cxx.h>
#include <slowPaging.h>

namespace arch
{   
    /**
     * @brief Expansion / contraction of kernel memory and allocation of physical pages.
    */
    class HeapManager
    {
        public:
            HeapManager(uintptr_t base, uintptr_t limit) : _currentBreakPoint(base), _base(base), _limit(limit){};

            /**
             * @brief Request an expansion / contraction of the kernel heap. 
             * @param bytes Number of additional / reduced bytes wanted on the heap.
             * @return The new heap break point in virtual memory.
            */
            uintptr_t RequestHeapBytes(intptr_t bytes);
        private:
            uintptr_t _currentBreakPoint; ///< The current kernel break point.

            uintptr_t _base;
            uintptr_t _limit;
            Spinlock _lock;
    };
}

#endif