#include <arch/machine.h>
#include <memorylayout.h>
#include <memorylayout.h>

namespace arch
{
    HeapManager Machine::_kernelHeapManager(MEMRANGE_HEAP, MEMRANGE_HEAP_LIMIT);
    MemoryAllocator Machine::_heapAllocator(_kernelHeapManager, KERNEL_HEAP_INCREMENT, PAGE_GLOBAL | PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE);
}