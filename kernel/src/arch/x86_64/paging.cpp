#include <stdint.h>
#include <stddef.h>
#include <paging.h>
#include <pagingstructures.h>
#include <memory/pageframeallocator.h>
#include <debug.h>
#include <x86_64_utilities.h>

namespace arch
{
    void Paging::PageIn4k( uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress )
    {
        uint64_t* pml4 = (uint64_t*)V_PML4_4K;
        uint64_t* pdpt = (uint64_t*)V_PDPT_4K(virtualAddress);
        uint64_t* pd = (uint64_t*)V_PD_4K(virtualAddress);
        uint64_t* pt = (uint64_t*)V_PT_4K(virtualAddress);

        uint64_t pml4i = PML4_INDEX(virtualAddress);
        uint64_t pdpti = PDPT_INDEX(virtualAddress);
        uint64_t pdi = PD_INDEX(virtualAddress);
        uint64_t pti = PT_INDEX(virtualAddress);

        if((flags & PAGE_LARGE) != 0) FATAL("Attempting to page in 4k page with large page flag set.");
        if((flags & PAGE_PRESENT) == 0) FATAL("Attempting to page in 4k page with present page flag not set.");
        if((flags & ~0xFFFULL) != 0) FATAL("Attempting to page in 4k page with high bits set.");

        if((pml4[pml4i] & PAGE_PRESENT) == 0)
        {
            uintptr_t newPdpt = PageFrameAllocator::GetInstance().AllocateEmpty();
            if(newPdpt == 0) FATAL("Unable to allocate physical memory for new PDPT.");
            pml4[pml4i] = newPdpt | PAGE_PRESENT | PAGE_WRITE;
            X86_64_Utilities::InvalidatePage((uintptr_t)pdpt);
        }

        if((pdpt[pdpti] & PAGE_PRESENT) == 0)
        {
            uintptr_t newPd = PageFrameAllocator::GetInstance().AllocateEmpty();
            if(newPd == 0) FATAL("Unable to allocate physical memory for new PD.");
            pdpt[pdpti] = newPd | PAGE_PRESENT | PAGE_WRITE;
            X86_64_Utilities::InvalidatePage((uintptr_t)pd);
        }

        if((pd[pdi] & PAGE_PRESENT) == 0)
        {
            uintptr_t newPt = PageFrameAllocator::GetInstance().AllocateEmpty();
            if(newPt == 0) FATAL("Unable to allocate physical memory for new PT.");            
            pd[pdi] = newPt | flags;
            X86_64_Utilities::InvalidatePage((uintptr_t)pt);
        }

        pt[pti] = physicalAddress | flags;
        X86_64_Utilities::InvalidatePage(virtualAddress);
    }
    
    void Paging::PageIn2m(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
    {
        uint64_t* pml4 = (uint64_t*)V_PML4_4K;
        uint64_t* pdpt = (uint64_t*) V_PDPT_4K(virtualAddress);
        uint64_t* pd = (uint64_t*) V_PD_4K(virtualAddress);

        uint64_t pml4i = PML4_INDEX(virtualAddress);
        uint64_t pdpti = PDPT_INDEX(virtualAddress);
        uint64_t pdi = PD_INDEX(virtualAddress);

        if((flags & PAGE_LARGE) == 0) FATAL("Attempting to page in 2m page with large page flag not set.");
        if((flags & PAGE_PRESENT) == 0) FATAL("Attempting to page in 4k page with present page flag not set.");
        if((flags & ~0xFFFULL) != 0) FATAL("Attempting to page in 4k page with high bits set.");

        if((pml4[pml4i] & PAGE_PRESENT) == 0)
        {
            uintptr_t newPdpt = PageFrameAllocator::GetInstance().AllocateEmpty();
            if(newPdpt == 0) FATAL("Unable to allocate physical memory for new PDPT.");
            pml4[pml4i] = newPdpt | PAGE_PRESENT | PAGE_WRITE;
            X86_64_Utilities::InvalidatePage((uintptr_t)pdpt);
        }

        if((pdpt[pdpti] & PAGE_PRESENT) == 0)
        {
            uintptr_t newPd = PageFrameAllocator::GetInstance().AllocateEmpty();
            if(newPd == 0) FATAL("Unable to allocate physical memory for new PD.");
            pdpt[pdpti] = newPd | PAGE_PRESENT | PAGE_WRITE;
            X86_64_Utilities::InvalidatePage((uintptr_t)pd);
        }

        pd[pdi] = physicalAddress | flags;
    }


} // namespace arch

