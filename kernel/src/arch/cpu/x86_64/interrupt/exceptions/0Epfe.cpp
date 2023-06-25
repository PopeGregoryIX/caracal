#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <interrupt/exceptions.h>
#include <registers.h>
#include <memory/memorylayout.h>
#include <memory/virtualmemorymanager.h>
#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>
#include <process/processmanager.h>

namespace arch
{
    Registers* Exceptions::PageFaultExceptionHandler(Registers* registers)
    {
        uintptr_t faultAddress = CPU_CLASS::ReadCr2();
        VirtualMemoryManager& virtualMemoryManager = VirtualMemoryManager::GetInstance();
        bool handled = false;
        //INFO("Page Fault Exception at: " << faultAddress << " with error code " << registers->errorCode);

        MemoryAllocator* allocator = virtualMemoryManager.IsAllocated(faultAddress);    
        if(allocator != nullptr)
        {
            //  This memory is known to be allocated - we can safely page in.
            if((faultAddress % 0x200000) != 0) faultAddress = faultAddress - (faultAddress % 0x200000);
            
            if(allocator == &VirtualMemoryManager::GetInstance().GetKernelAllocator())
            {
                CPU_CLASS::PageInLarge(PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL, faultAddress);
                handled = true;
            }
        }

        
        if(!handled)
        {
            WARNING("PFE unhandled. Dumping core:");

            if(faultAddress < 0x200000ULL)
            {   WARNING("This was probably a null-reference exception\n\n");    }
            else if (faultAddress < PDPTE_RANGE)
            {   WARNING("This seems to be in physical memory space\n\n");  }
            else if (faultAddress < (PDPTE_RANGE * 2))
            {   WARNING("This seems to be in user space\n\n");  }
            else if(faultAddress < SUPERVISOR_THREAD_STACK && faultAddress > (SUPERVISOR_THREAD_STACK - PML4E_RANGE))
            { WARNING("This seems to be in supervisor stack space\n\n"); }
            else if(faultAddress > (UINT64_MAX - (PDPT_RANGE * 2)))
            {   WARNING("This seems to be in kernel space\n\n");    }
            else
            {   WARNING("No Suggestions as to what caused this\n\n");   }

            Exceptions::DumpCore(registers);
            FATAL("Unhandled PFE");            
        }

        return registers;
    }
}
