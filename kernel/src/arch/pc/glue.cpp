/**
 * \file glue.cpp
 * 
 * The header for the functions below are the same for each
 * architecture, but the glue.cpp file is different per-architecture.
 * 
 * This code links the arch-dependent and arch-independent portions of the kernel.
*/
#include <arch/glue.h>
#include <arch/cpu.h>
#include <arch/machine.h>
#include <x86_64.h>

namespace arch
{
    void Glue::EarlyMemorySetup(CBoot& cboot)
    {
        //  kmain already ensures that only the BSP calls this function.
        X86_64::IdtSetup();
        X86_64::PageFrameAllocationSetup(cboot);
        X86_64::PagingSetup(cboot);

        //  From now on, the bitmap page frame allocator and kernel pager (not "SlowPaging!") should be used.
        Machine::GetHeapAllocator().Initialise();
    }

    void Glue::APSetup(CBoot& cboot)
    {
        X86_64::APSetup(cboot);
    }
}
