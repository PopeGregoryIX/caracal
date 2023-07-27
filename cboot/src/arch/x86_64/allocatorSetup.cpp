#include <stdint.h>
#include <stddef.h>

#include <memory/memoryarray.h>
#include <paging.h>
#include <cpuutilities.h>

namespace arch
{
    void AllocatorSetup(MemoryArray& mmap)
    {
        Paging::SetGetPageFunction(mmap.AllocateMemorySmall);
        CpuUtilities::SetPageRoutines(mmap.AllocateMemorySmall, mmap.AllocateMemoryLarge);
    }
}