#ifndef __PAGING__H__
#define __PAGING__H__

#include <stdint.h>
#include <stddef.h>
#include <memory/spinlock.h>
#include <pagingstructures.h>

namespace arch
{
    class SlowPaging
    {
        public:
            static inline void SetGetPageFunction(uintptr_t (*pfunc)( void )) { _getPagingStructure = pfunc; }

            static bool IsPagedIn(uintptr_t virtualAddress);
            static void PageIn4k( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );
            static void PageIn2m( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress );
        private:
            static Spinlock _pageLock;

            static uintptr_t (*_getPagingStructure)( void );

    };
}

#endif
