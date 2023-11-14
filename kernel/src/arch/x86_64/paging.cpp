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

        INFO("Page in at " << virtualAddress);
        INFO("PML4I " << pml4i);
        INFO("PDPTI " << pdpti);
        INFO("PDI " << pdi);
        INFO("PTI " << pti);
        
        INFO("PML4 " << (uint64_t)pml4);
        INFO("PDPT " << (uint64_t)pdpt);
        INFO("PD " << (uint64_t)pd);
        INFO("PT " << (uint64_t)pt);

        if((flags & PAGE_LARGE) != 0) FATAL("Attempting to page in 4k page with large page flag set.");

        pt[PT_INDEX(virtualAddress)] = physicalAddress | flags;

        FATAL("Page 4K");
    }
    
    void Paging::PageIn2m(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
    {
        uint64_t* pml4 = (uint64_t*)V_PML4_4K;
        uint64_t* pdpt = (uint64_t*) V_PDPT_4K(virtualAddress);
        uint64_t* pd = (uint64_t*) V_PD_4K(virtualAddress);

        INFO("Virtual address is at: " << virtualAddress);
        INFO("PML4: " << (uint64_t)pml4 << "  :  PDPT: " << (uint64_t)pdpt << "  :  PD:" << (uint64_t)pd);

        FATAL("Need to write paging routine!");
    }


} // namespace arch

