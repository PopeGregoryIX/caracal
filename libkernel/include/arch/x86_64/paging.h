#ifndef __KERNEL__PAGING__H__
#define __KERNEL__PAGING__H__

#include <stdint.h>
#include <stddef.h>
#include <memory/pageframeallocator.h>

class Paging
{
    public:
        static inline uintptr_t Allocate4kPage( void ) 
        { 
            uintptr_t page =  PageFrameAllocator::GetInstance().Allocate(0x1000); 
            if(page == 0 || page == UINT64_MAX) FATAL("Out of memory for physical RAM allocation.");
            return page;
        }

        static void PageIn4k(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress);
    protected:

}

#endif
