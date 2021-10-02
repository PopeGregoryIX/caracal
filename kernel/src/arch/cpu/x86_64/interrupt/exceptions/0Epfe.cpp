#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <interrupt/exceptions.h>
#include <registers.h>
#include <memory/memorylayout.h>
#include <memory/virtualmemorymanager.h>
#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>

namespace arch
{
    Registers* Exceptions::PageFaultExceptionHandler(Registers* registers)
    {
        uintptr_t faultAddress = CPU_CLASS::ReadCr2();
        VirtualMemoryManager& virtualMemoryManager = VirtualMemoryManager::GetInstance();
        bool handled = false;
        INFO("Page Fault Exception at: " << faultAddress << " with error code " << registers->errorCode);

        MemoryAllocator* allocator = virtualMemoryManager.IsAllocated(faultAddress);                
        if(allocator != nullptr)
        {
            //  This memory is known to be allocated - we can safely page in.
            pageDirectoryEntry_t* pml4 = (pageDirectoryEntry_t*)CPU_CLASS::ReadCr3();
            if((faultAddress % 0x200000) != 0) faultAddress = faultAddress - (faultAddress % 0x200000);

            if(allocator == &VirtualMemoryManager::GetInstance().GetKernelAllocator())
            {
                pageDirectoryEntry_t* pdpt = (pageDirectoryEntry_t*)(pml4[PML4_INDEX(faultAddress)].Address());
                if(pdpt == nullptr)
                    pdpt = 
                    (pageDirectoryEntry_t*)((((uint64_t*)pml4)[PML4_INDEX(faultAddress)] = (uint64_t)HeapManager::GetNewPagingStructure() | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE) & ~0xFFFULL);

                pageDirectoryEntry_t* pd = (pageDirectoryEntry_t*)(pdpt[PDPT_INDEX(faultAddress)].Address());
                if(pd == nullptr) 
                    pd = 
                        (pageDirectoryEntry_t*)((((uint64_t*)pdpt)[PDPT_INDEX(faultAddress)] = (uint64_t)HeapManager::GetNewPagingStructure() | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE) & ~0xFFFULL);

                INFO("PDPT: " << (uint64_t)pdpt);
                INFO("PD: " << (uint64_t)pd);
                INFO("INDEX: " << (uint64_t)PD_INDEX(faultAddress));
                INFO("Address:" << pd[PD_INDEX(faultAddress)].Address());

                if(pd[PD_INDEX(faultAddress)].Address() == 0)
                    ((uint64_t*)pd)[PD_INDEX(faultAddress)] = PageFrameAllocator::GetInstance().Allocate(0x200000) | 
                        PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL;
            }

            handled = true;
        }
        
        if(!handled)
        {
            INFO(" ");
            WARNING("PFE unhandled. Dumping core:");

            if(faultAddress < 0x200000ULL)
            {   WARNING("This was probably a null-reference exception\n\n");    }
            else if (faultAddress < PDPTE_RANGE)
            {   WARNING("This seems to be in physical memory space\n\n");  }
            else if (faultAddress < (PDPTE_RANGE * 2))
            {   WARNING("This seems to be in user space\n\n");  }
            else if(faultAddress > (UINT64_MAX - PDPTE_RANGE))
            {   WARNING("This seems to be in kernel space\n\n");    }
            else
            {   WARNING("No Suggestions as to what caused this\n\n");   }
            

            Exceptions::DumpCore(registers);
            
            FATAL("Unhandled PFE");
        }

        return registers;
    }
}
