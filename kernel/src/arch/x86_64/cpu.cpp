#include <cboot.h>
#include <stdint.h>
#include <stddef.h>
#include <cpu.h>
#include <cpuutilities.h>
#include <memory/memoryarray.h>
#include <memory/pageframeallocator.h>
#include <paging.h>

namespace arch
{
    void Cpu::EarlyMemorySetup( CBoot& cboot )
    {
        Paging::SetGetPageFunction(MemoryArray::AllocateMemorySmall);
        CpuUtilities::SetPageRoutines( MemoryArray::AllocateMemorySmall, MemoryArray::AllocateMemoryLarge );

        MemoryArray& mmap = MemoryArray::GetInstance();
		mmap.Initialise(cboot.mmapAddress, cboot.mmapBytes);
        
		PageFrameAllocator::GetInstance().Initialise(0x1000, mmap);
    }
}
