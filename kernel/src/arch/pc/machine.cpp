#include <arch/machine.h>
#include <memorylayout.h>

namespace arch
{
    MemoryAllocator Machine::_heapAllocator(KERNEL_HEAP_INCREMENT);
}