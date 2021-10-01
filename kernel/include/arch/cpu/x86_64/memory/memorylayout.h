#ifndef __MEMORYLAYOUT__H__
#define __MEMORYLAYOUT__H__

/**
 *  X86-64 Long-Mode Memory Map
 * 
 *  * 0xFFFF FF80 0000 0000 - 0xFFFF FFFF FFFF FFFF
 *      * 0xFFFF FFFF F000 0000 : Memory Mapped IO
 *      * 0xFFFF FFFF F400 0000 : Linear Frame Buffer (up to x F5FA 4000 for 4k resolution, so plenty of space!)
 *      * 0xFFFF FFFF F800 0000 : BootBoot Structure
 *      * 0xFFFF FFFF F800 1000 : BootBoot Environment Variables
 *      * 0xFFFF FFFF F800 2000 : Kernel Code and Data
 *      * 0xFFFF FFFF FE00 0000 : Kernel Stack
 * Default BootBoot values have been changed from their regular values to
 * allow for extra "headroom". This allows, for example, for the kernel to
 * be mapped with large 2GiB pages, but with a guard page between the
 * stack and the kernel code.
*/

namespace arch
{
    //  PT Covers 2 MiB
    #define PTE_RANGE   0x1000ULL
    #define PT_RANGE    0x200000ULL
    
    //  PD Covers 1GiB
    #define PDE_RANGE   0x200000ULL
    #define PD_RANGE    0x40000000ULL

    //  PDPT Covers 512GiB
    #define PDPTE_RANGE 0x40000000ULL
    #define PDPT_RANGE  0x8000000000ULL

    //  Entire PML4 covers 256TiB
    #define PML4E_RANGE 0x8000000000ULL
    #define PML4_RANGE  0x10000000000000ULL

    #define PML4_Entry(Address) ((Address) / PML4E_RANGE)
    #define PD_INDEX(Address) (((Address) >> 21) & 0x1FFULL)

    #define PAGE_PRESENT        (1ULL)
    #define PAGE_WRITE          (1ULL << 1)
    #define PAGE_USER           (1ULL << 2)
    #define PAGE_WRITETHROUGH   (1ULL << 3)
    #define PAGE_CACHEDISABLE   (1ULL << 4)
    #define PAGE_ACCESSED       (1ULL << 5)
    #define PAGE_DIRTY          (1ULL << 6)
    #define PAGE_LARGE          (1ULL << 7)
    #define PAGE_GLOBAL         (1ULL << 8)

    #define KERNEL_HEAP ((uint64_t)((0xFFFFFFFFFFFFFFFFULL - (PDPT_RANGE * 2)) + 1))

    #define KERNEL_HEAP_SIZE (PDPT_RANGE)
}

#endif