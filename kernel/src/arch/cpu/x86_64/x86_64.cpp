/*
 * x86_64.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <stdint.h>
#include <stddef.h>
#include<cpu.h>
#include<x86_64.h>
#include <archdef.h>
#include<bootboot.h>
#include <cpuid.h>
#include <debug/debug.h>
#include <memory/memorylayout.h>
#include <memory/heapmanager.h>

bool Cpu::IsBsp( void )
{
	return ProcessorId() == bootboot.bspid;
}

unsigned int Cpu::ProcessorId()
{
	volatile uint32_t discard, ebx;
	__cpuid(1, discard, ebx, discard, discard);

	return ((ebx >> 24) & 0xFF);
}

namespace arch
{

	void X86_64::PageInLarge(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
	{
		INFO("Page In Virtual: " << virtualAddress << " physical " << physicalAddress << " flags " << flags);
		uint64_t* pml4 = (uint64_t*)CPU_CLASS::ReadCr3();
		
		if(virtualAddress == UINT64_MAX) FATAL("Unable to page in - no physical memory allocated.");
		if((virtualAddress % 0x200000ULL) != 0) FATAL("Please align virtual address before paging in! At:" << virtualAddress);
		if((physicalAddress % 0x200000ULL) != 0) FATAL("Please align physical address before paging in! At:" << physicalAddress);

		uint64_t* pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pdpt == nullptr)
		{
			pml4[PML4_INDEX(virtualAddress)] = ((uint64_t)HeapManager::GetNewPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t* pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pd == nullptr)
		{
			pdpt[PDPT_INDEX(virtualAddress)] = ((uint64_t)HeapManager::GetNewPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
			pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
		}

		uint64_t pde = (pd[PD_INDEX(virtualAddress)] & ~0xFFFULL);
		if(pde == 0)
			pd[PD_INDEX(virtualAddress)] = physicalAddress | flags;

		InvalidatePage(virtualAddress);
	}
}


