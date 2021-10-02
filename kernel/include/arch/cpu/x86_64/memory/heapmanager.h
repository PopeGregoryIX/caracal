#ifndef __HEAPMANAGER_H_
#define __HEAPMANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/pageframeallocator.h>
#include <debug/debug.h>
#include <runtime/cxx.h>

namespace arch
{   
    class HeapManager
    {
        public:
            static uintptr_t RequestKernelHeapBytes(intptr_t bytes);

            static pageDirectoryEntry_t* GetNewPagingStructure( void )
            {
                uintptr_t pageStructure = PageFrameAllocator::GetInstance().Allocate();
                if(pageStructure == UINT64_MAX) FATAL("Unable to find memory for a new paging structure.");

                ::memset((void*)pageStructure, 0, 0x1000);

                return (pageDirectoryEntry_t*)pageStructure;
            }
        private:
            static uintptr_t _kernelHeapBreak;
    };
}

#endif
