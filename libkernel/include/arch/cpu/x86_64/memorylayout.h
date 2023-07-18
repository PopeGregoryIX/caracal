#ifndef __MEMORYLAYOUT__H__
#define __MEMORYLAYOUT__H__

/**
 *  X86-64 Long-Mode Memory Map
 * 
 * USER SPACE
 *      * 0x0000 0000 0000 0000 : NULL trap
 *      * 0x0000 0000 0020 0000 : Code and data (different per TASK)
 *      * 0x0000 0008 0000 0000 : User-mode stack (different per THREAD)
 *      * 0x0000 0800 0000 0000 : Kernel-mode stack (different per THREAD)
 *
 * KERNEL SPACE
 *      * 0xFFFF FE00 0000 0000 : Physical RAM mirror
 *      * 0xFFFF FFFF 0000 0000 : Memory Mapped IO
 *      * 0xFFFF FFFF 8000 0000 : Kernel Heap (Memory top - 2xPDPT entries)
 *      * 0xFFFF FFFF E000 0000 : unallocated
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
    const uintptr_t MEMRANGE_LFB=0xFFFFFE8000000000ULL;
    const uintptr_t MEMRANGE_DATA=0xFFFFFE8000600000ULL;
    const uintptr_t MEMRANGE_MMAP=0xFFFFFE8000600000ULL;
    const uintptr_t MEMRANGE_CBOOT=0xFFFFFE8000608000ULL;
    const uintptr_t MEMRANGE_CONFIG=0xFFFFFE8000609000ULL;
    const uintptr_t MEMRANGE_GDT=0xFFFFFE8000A00000ULL;
    const uintptr_t MEMRANGE_MEMBITMAP=0xFFFFFE8000E00000ULL;
    const uintptr_t MEMRANGE_STACKS=0xFFFFFEF000000000;
    
    //  PT Covers 2 MiB
    #define PTE_RANGE   0x1000ULL
    #define PT_RANGE    0x200000ULL
    #define PT_INDEX(Address) (((Address) >> 12) & 0x1FFULL)
    
    //  PD Covers 1GiB
    #define PDE_RANGE   0x200000ULL
    #define PD_RANGE    0x40000000ULL
    #define PD_INDEX(Address) (((Address) >> 21) & 0x1FFULL)

    //  PDPT Covers 512GiB
    #define PDPTE_RANGE 0x40000000ULL
    #define PDPT_RANGE  0x8000000000ULL
    #define PDPT_INDEX(Address) (((Address) >> 30) & 0x1FFULL)

    //  Entire PML4 covers 256TiB
    #define PML4E_RANGE 0x8000000000ULL
    #define PML4_RANGE  0x10000000000000ULL
    #define PML4_INDEX(Address) (((Address) >> 39) & 0x1FFULL)

    #define PML4_Entry(Address) ((Address) / PML4E_RANGE)
    
    #define PAGE_PRESENT        (1ULL)
    #define PAGE_WRITE          (1ULL << 1)
    #define PAGE_USER           (1ULL << 2)
    #define PAGE_WRITETHROUGH   (1ULL << 3)
    #define PAGE_CACHEDISABLE   (1ULL << 4)
    #define PAGE_ACCESSED       (1ULL << 5)
    #define PAGE_DIRTY          (1ULL << 6)
    #define PAGE_LARGE          (1ULL << 7)
    #define PAGE_GLOBAL         (1ULL << 8)

    #define KERNEL_HEAP ((uint64_t)((UINT64_MAX - (PDPT_RANGE * 2)) + 1))
    #define KERNEL_HEAP_SIZE (PDPT_RANGE)
    #define KERNEL_HEAP_MAX ((KERNEL_HEAP + KERNEL_HEAP_SIZE) - 1)

    #define KERNEL_HEAP_INCREMENT 0x200000
    #define KERNEL_HEAP_MINSPLIT  0x1000

	#define USER_THREAD_STACK	  			0x0000000800000000
	#define SUPERVISOR_THREAD_STACK			(0x0000800000000000 - 0x100000)
}

#endif
