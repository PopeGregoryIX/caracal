#include <stdint.h>
#include <stddef.h>
#include <bootboot.h>
#include <memory/memoryarray.h>
#include <slowPaging.h>
#include <arch/cpuutilities.h>

namespace arch
{
    void AllocatorSetup()
    {
        MemoryArray::GetInstance().Initialise(&(bootboot.mmap), bootboot.size - sizeof(BOOTBOOT));
        
        SlowPaging::SetGetPageFunction(MemoryArray::AllocateMemorySmall);
        CpuUtilities::SetPageRoutines(MemoryArray::AllocateMemorySmall, MemoryArray::AllocateMemoryLarge);
    }
}