#ifndef __X__PAGING__H
#define __X__PAGING__H

#include <stdint.h>
#include <stddef.h>
#include <memory/spinlock.h>
#include <pagingstructures.h>

namespace arch
{
    class Paging
    {
        public:
            static bool IsPagedIn(uintptr_t virtualAddress);
            static void PageIn4k( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );
            static void PageIn2m( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );

        private:
            static Spinlock _pageLock;
    };
}

#endif