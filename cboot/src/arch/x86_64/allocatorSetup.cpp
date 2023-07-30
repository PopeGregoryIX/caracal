#include <stdint.h>
#include <stddef.h>
#include <bootboot.h>
#include <memory/memoryarray.h>
#include <paging.h>
#include <cpuutilities.h>

namespace arch
{
    void AllocatorSetup()
    {
        MemoryArray::GetInstance().Initialise(&(bootboot.mmap), bootboot.size - sizeof(BOOTBOOT));
        
        Paging::SetGetPageFunction(MemoryArray::AllocateMemorySmall);
        CpuUtilities::SetPageRoutines(MemoryArray::AllocateMemorySmall, MemoryArray::AllocateMemoryLarge);
    }
}