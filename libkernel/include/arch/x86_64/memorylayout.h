#ifndef __MEMORYLAYOUT__H__
#define __MEMORYLAYOUT__H__

/**
 *  X86-64 Long-Mode Memory Map
 * 
 * USER SPACE
 *      * 0x0000 0000 0000 0000 : NULL trap
 *      * 0x0000 0000 0020 0000 : Code and data (different per TASK)
 *      * 0x0000 0004 0000 0000 : User-mode stack (different per THREAD)
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
    //  Fixed Values

    // USER
    #define MEMRANGE_NULL                       0x0000000000000000ULL
    #define MEMRANGE_USERCODE                   0x0000000000200000ULL
    #define MEMRANGE_USERSTACK                  0x0000000400000000ULL
    #define MEMRANGE_USERSTACK_TOP(threadId)    ((MEMRANGE_USERSTACK + 0x00000003FFFFFFFFULL) - (0x200000ULL * (threadId)))
    #define MEMRANGE_USERSTACK_INITIAL(threadId)    (MEMRANGE_USERSTACK_TOP(threadId) - 0x0FFF)

    #define MEMRANGE_SHARED             0x0000000800000000ULL
    #define MEMRANGE_KERNELSTACK        0x0000040000000000ULL
    #define MEMRANGE_KERNELSTACK_TOP    0x00007FFFFFFFFFF8ULL
    

    //  KERNEL
    #define MEMRANGE_SUPERVISOR     0xFFFF800000000000ULL
    #define MEMRANGE_PHYSICAL       0xFFFFFA0000000000ULL
    #define MEMRANGE_LFB            0xFFFFFE8000000000ULL
    #define MEMRANGE_KERNEL         0xFFFFFE8100000000ULL
    #define MEMRANGE_DATA           0xFFFFFE8000600000ULL
    #define MEMRANGE_MMAP           0xFFFFFE8000600000ULL
    #define MEMRANGE_CBOOT          0xFFFFFE8000608000ULL
    #define MEMRANGE_CONFIG         0xFFFFFE8000609000ULL
    #define MEMRANGE_GDT            0xFFFFFE8000A00000ULL
    #define MEMRANGE_INITRD         0xFFFFFE8001000000ULL
    #define MEMRANGE_BITMAP         0xFFFFFE8040000000ULL
    #define MEMRANGE_STACKS         0xFFFFFEF000000000ULL
    #define MEMRANGE_HEAP           0xFFFFFF0000000000ULL
    #define MEMRANGE_HEAP_LIMIT     0xFFFFFF7000000000ULL
    #define MEMRANGE_DFE_STACK      0xFFFFFF7FFFFFF000ULL
    #define MEMRANGE_DFE_STACK_TOP  0xFFFFFF7FFFFFFFF8ULL
    #define MEMRANGE_PAGING         0xFFFFFF8000000000ULL

    #define MEMRANGE_STACK_BASE(cpuNum) (MEMRANGE_STACKS - ((cpuNum) * 0x400000))
    #define MEMRANGE_STACK_TOP(cpuNum) ((MEMRANGE_STACK_BASE(cpuNum)) + (0x200000 - 0x08))

    #define KERNEL_HEAP_SIZE (MEMRANGE_HEAP_LIMIT - MEMRANGE_HEAP)
    #define KERNEL_HEAP_MAX (MEMRANGE_HEAP_LIMIT - 1)

    #define KERNEL_HEAP_INCREMENT 0x200000
    #define KERNEL_HEAP_MINSPLIT  0x100

	#define USER_THREAD_STACK	  			0x0000000800000000
	#define SUPERVISOR_THREAD_STACK			(0x0000800000000000 - 0x100000)

    #define GET_VIRTUAL_POINTER(physicalAddress)   ((void*)(((uintptr_t)physicalAddress) + MEMRANGE_PHYSICAL))
}

#endif
