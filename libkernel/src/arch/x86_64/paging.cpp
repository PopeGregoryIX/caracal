#include <debug.h>
#include <stdint.h>
#include <stddef.h>
#include <paging.h>
#include <x86_64.h>
#include <spinlock.h>

namespace arch
{
    Spinlock Paging::_pageLock;
    uintptr_t (*Paging::_getPagingStructure)();

    bool Paging::IsPagedIn(uintptr_t virtualAddress)
    {
        uint64_t* pml4 = (uint64_t*)X86_64::ReadCr3();
		INFO("PML4 " << (uintptr_t)pml4);
        if((virtualAddress % 0x1000) != 0) virtualAddress-= (virtualAddress % 0x1000);
        INFO("b");
		uint64_t* pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		INFO("PDPT " << (uintptr_t)pdpt);
        if(pdpt == nullptr) return false;
		INFO("c");
        uint64_t* pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		INFO("PD " << (uintptr_t)pd);
        if(pd == nullptr) return false;

		INFO("d");
        uint64_t pt = (pd[PD_INDEX(virtualAddress)]);

        if((pt & PAGE_LARGE) == PAGE_LARGE) 
            return true;
        else
            return ((pt & ~0xFFFULL) != 0);
    }

    void Paging::PageIn2m(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
	{
		uint64_t* pml4 = (uint64_t*)X86_64::ReadCr3();

		if(virtualAddress == UINT64_MAX) FATAL("Unable to page in - no physical memory allocated.");
		if((virtualAddress % 0x200000ULL) != 0) FATAL("Please align virtual address before paging in! At:" << virtualAddress);
		if((physicalAddress % 0x200000ULL) != 0) FATAL("Please align physical address before paging in! At:" << physicalAddress);

		_pageLock.Acquire();
		if((flags & PAGE_LARGE) != PAGE_LARGE) flags|= PAGE_LARGE;

		uint64_t* pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pdpt == nullptr)
		{
			pml4[PML4_INDEX(virtualAddress)] = ((uint64_t)_getPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t* pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pd == nullptr)
		{
			pdpt[PDPT_INDEX(virtualAddress)] = ((uint64_t)_getPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t pde = (pd[PD_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pde == 0)
			pd[PD_INDEX(virtualAddress)] = physicalAddress | flags;

		X86_64::InvalidatePage(virtualAddress);

		_pageLock.Release();
	}

	void Paging::PageIn4k(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
	{
		uint64_t* pml4 = (uint64_t*)X86_64::ReadCr3();
		
		if(virtualAddress == UINT64_MAX) FATAL("Unable to page in - no physical memory allocated.");
		if((virtualAddress % 0x1000ULL) != 0) FATAL("Please align virtual address before paging in! At:" << virtualAddress);
		if((physicalAddress % 0x1000ULL) != 0) FATAL("Please align physical address before paging in! At:" << physicalAddress);

		_pageLock.Acquire();
		if((flags & PAGE_LARGE) == PAGE_LARGE) flags&= (~PAGE_LARGE & 0xFFF);

		uint64_t* pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pdpt == nullptr)
		{
			pml4[PML4_INDEX(virtualAddress)] = ((uint64_t)_getPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t* pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pd == nullptr)
		{
			pdpt[PDPT_INDEX(virtualAddress)] = ((uint64_t)_getPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t* pt = (uint64_t*)(pd[PD_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pt == nullptr)
		{
			pd[PD_INDEX(virtualAddress)] = ((uint64_t)_getPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pt = (uint64_t*)(pd[PD_INDEX(virtualAddress)] & ~0xFFFULL);
		}
		
		uint64_t pte = (pt[PT_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pte == 0)
			pt[PT_INDEX(virtualAddress)] = physicalAddress | flags;

		X86_64::InvalidatePage(virtualAddress);

		_pageLock.Release();
	}
}