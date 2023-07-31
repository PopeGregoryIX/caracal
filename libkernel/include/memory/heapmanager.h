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
            /**
             * @brief Request an expansion / contraction of the kernel heap. 
             * @param bytes Number of additional / reduced bytes wanted on the heap.
             * @return The new heap break point in virtual memory.
            */
            static uintptr_t RequestKernelHeapBytes(intptr_t bytes);
        private:
            static uintptr_t _kernelHeapBreak; ///< The current kernel break point.
    };
}

#endif