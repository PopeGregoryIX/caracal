#include <stdint.h>
#include <stddef.h>
#include <exceptions.h>
#include <registers.h>
#include <debug.h>
#include <x86_64_utilities.h>
#include <memorylayout.h>
#include <arch/machine.h>
#include <paging.h>

namespace arch
{
    const char* PageFaultExceptionReason(uintptr_t pfeLocation);

    Registers* Exceptions::PageFaultExceptionHandler(Registers* registers)
    {
        uintptr_t pfeLocation = X86_64_Utilities::ReadCr2();
        bool handled = false;

        if(Machine::GetHeapAllocator().IsAllocated(pfeLocation))
        {
            INFO("Paging In");
            Paging::PageIn2m(Machine::GetHeapAllocator().GetMemoryType(), pfeLocation, PageFrameAllocator::GetInstance().Allocate(0x200000));
            handled = true;
        }

        if(!handled)
        {
            Exceptions::DumpCore(registers);
            INFO("Further PFE information:\n" << PageFaultExceptionReason(pfeLocation))
            FATAL("Unhandled PFE at " << X86_64_Utilities::ReadCr2());
        }

        return registers;
    }

    const char* PageFaultExceptionReason(uintptr_t pfeLocation)
    {
        if(pfeLocation < 0x200000)
            return "NULL reference exception?";
        else if(pfeLocation < MEMRANGE_SUPERVISOR)
            return "PFE was in user space.";
        else if(pfeLocation >= MEMRANGE_PAGING)
            return "PFE occurred in paging structures (unexpected attempted page in?)";
        else if (pfeLocation >= MEMRANGE_HEAP_LIMIT)
            return "PFE somewhere above the kernel heap in a guard area (buffer overflow?).";
        else if (pfeLocation >= MEMRANGE_HEAP)
            return "PFE somewhere in the kernel heap (buffer overflow?).";
        else if (pfeLocation >= MEMRANGE_STACKS)
            return "PFE somewhere in initial kernel stacks (stack overflow?)";
        else if (pfeLocation >= MEMRANGE_BITMAP)
            return "PFE somewhere in physical memory bitmap (overflow?)";
        else if (pfeLocation >= MEMRANGE_INITRD)
            return "PFE somewhere in initRd area (overflow?)";
        else if (pfeLocation >= MEMRANGE_GDT)
            return "PFE somewhere in GDT area (overflow?)";
        else if (pfeLocation >= MEMRANGE_CONFIG)
            return "PFE somewhere in Config File area (overflow?)";
        else if (pfeLocation >= MEMRANGE_CBOOT)
            return "PFE somewhere in CBoot structure area (overflow?)";
        else if (pfeLocation >= MEMRANGE_MMAP)
            return "PFE somewhere in CBoot Memory Map structure area (overflow?)";
        else if (pfeLocation >= MEMRANGE_KERNEL)
            return "PFE somewhere in Kernel Code area (static value overflow?)";
        else if (pfeLocation >= MEMRANGE_LFB)
            return "PFE somewhere in LFB area (console overflow?)";

        return "Unknown";
    }
}