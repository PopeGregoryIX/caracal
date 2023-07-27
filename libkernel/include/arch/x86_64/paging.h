#ifndef __PAGING__H__
#define __PAGING__H__

#include <stdint.h>
#include <stddef.h>

namespace arch
{
    class Paging
    {
        public:
            static bool IsPagedIn(uintptr_t virtualAddress);
            static void PageIn4k( uintptr_t virtualAddress, uintptr_t physicalAddress );
            static void PageIn2m( uintptr_t virtualAddress, uintptr_t physicalAddress );
    };
}

#endif
