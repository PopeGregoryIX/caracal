#ifndef __X__PAGING_STRUCTS_H
#define __X__PAGING_STRUCTS_H

#include <stdint.h>
#include <stddef.h>

namespace arch
{
    //  PT Covers 2 MiB
    #define PTE_RANGE   0x1000ULL
    #define PT_RANGE    0x200000ULL
    #define PT_INDEX(Address) (((Address) >> 12) & 0x1FFULL)
    #define V_PT_4K(Address)  (0xFFFFFF8000000000 + (0x40000000 * PDPT_INDEX(Address)) + (0x200000 * PD_INDEX(Address)) + (0x1000 * PT_INDEX(Address)))

    //  PD Covers 1GiB
    #define PDE_RANGE   0x200000ULL
    #define PD_RANGE    0x40000000ULL
    #define PD_INDEX(Address) (((Address) >> 21) & 0x1FFULL)
    #define V_PD_4K(Address)  (0xFFFFFFFFC0000000 + (0x200000 * PDPT_INDEX(Address)) + (0x1000 * PD_INDEX(Address)))

    //  PDPT Covers 512GiB
    #define PDPTE_RANGE 0x40000000ULL
    #define PDPT_RANGE  0x8000000000ULL
    #define PDPT_INDEX(Address) (((Address) >> 30) & 0x1FFULL)
    #define V_PDPT_4K(Address)  (0xFFFFFFFFFFE00000	+ (0x1000 * PDPT_INDEX(Address)))

    //  Entire PML4 covers 256TiB
    #define PML4E_RANGE 0x8000000000ULL
    #define PML4_RANGE  0x10000000000000ULL
    #define PML4_INDEX(Address) (((Address) >> 39) & 0x1FFULL)
    #define PML4_Entry(Address) ((Address) / PML4E_RANGE)
    #define V_PML4_4K (0xFFFFFFFFFFFFF000)


    #define PAGE_PRESENT        (1ULL)
    #define PAGE_WRITE          (1ULL << 1)
    #define PAGE_USER           (1ULL << 2)
    #define PAGE_WRITETHROUGH   (1ULL << 3)
    #define PAGE_CACHEDISABLE   (1ULL << 4)
    #define PAGE_ACCESSED       (1ULL << 5)
    #define PAGE_DIRTY          (1ULL << 6)
    #define PAGE_LARGE          (1ULL << 7)
    #define PAGE_GLOBAL         (1ULL << 8)

    struct pageTableEntry
    {
        uint64_t present:1;
        uint64_t writable:1;
        uint64_t user:1;
        uint64_t writeThrough:1;
        uint64_t cacheDisable:1;
        uint64_t accessed:1;
        uint64_t dirty:1;
        uint64_t zero:1;
        uint64_t global:1;
        uint64_t available:3;
        uint64_t page:52;

        inline uint64_t Address() { return page  * 0x1000;  }
    };

    struct pageDirectoryEntry
    {
        uint64_t present:1;
        uint64_t writable:1;
        uint64_t user:1;
        uint64_t writeThrough:1;
        uint64_t cacheDisable:1;
        uint64_t accessed:1;
        uint64_t dirty:1;
        uint64_t size:1;
        uint64_t global:1;
        uint64_t available:3;
        uint64_t page:52;

        inline uint64_t Address() { return page  * 0x1000;  }
        inline void Set(uint64_t Address, bool pres, bool write, bool large = false)
        {
            uint64_t* tmp = (uint64_t*)this;
            *tmp = (Address & ~0xFFF);
            present = (pres) ? 1 : 0;
            writable = (write) ? 1 : 0;
            user = 0;
            writeThrough = 0;
            cacheDisable = 0;
            accessed = 0;
            dirty = 0;
            size = (large) ? 1 : 0;
            global = 0;
            available = 0;
        }    
    };
}

#endif
