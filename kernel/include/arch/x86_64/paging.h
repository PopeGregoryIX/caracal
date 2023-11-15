#ifndef __X__PAGING__H
#define __X__PAGING__H

#include <stdint.h>
#include <stddef.h>
#include <memory/spinlock.h>
#include <pagingstructures.h>
#include <memory/pageframeallocator.h>

namespace arch
{
    class Paging
    {
        public:
            static bool IsPagedIn(uintptr_t virtualAddress);
            inline static void PageIn4k( uintptr_t flags, uintptr_t virtualAddress) 
                { PageIn4k(flags, virtualAddress, PageFrameAllocator::GetInstance().Allocate(0x1000)); }
                
            static void PageIn4k( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );

            inline static void PageIn2m( uintptr_t flags, uintptr_t virtualAddress) 
                { PageIn2m(flags, virtualAddress, PageFrameAllocator::GetInstance().Allocate(0x200000, 0x200000)); }
            static void PageIn2m( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );

        private:
            static Spinlock _pageLock;
    };
}

#endif