#include <stdint.h>
#include <stddef.h>
#include <paging.h>
#include <pagingstructures.h>
#include <memory/pageframeallocator.h>
#include <debug.h>
#include <x86_64_utilities.h>

namespace arch
{
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

