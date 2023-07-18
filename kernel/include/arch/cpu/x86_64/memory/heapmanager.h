#ifndef __HEAPMANAGER_H_
#define __HEAPMANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/pageframeallocator.h>
#include <debug/debug.h>
#include <cxx.h>

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

            /**
             * @brief Paging structure are 0x1000-aligned blocks of memory. Request a new structure which will be zeroed.
             * @return A pointer to a new paging stucture, or nullptr if failed.
            */
            static pageDirectoryEntry_t* GetNewPagingStructure( void )
            {
                uintptr_t pageStructure = PageFrameAllocator::GetInstance().Allocate();
                if(pageStructure == UINT64_MAX) FATAL("Unable to find memory for a new paging structure.");

                ::memset((void*)pageStructure, 0, 0x1000);

                return (pageDirectoryEntry_t*)pageStructure;
            }
        private:
            static uintptr_t _kernelHeapBreak; ///< The current kernel break point.
    };
}

#endif
