/**
 * @file x86_64.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <stdint.h>
#include <stddef.h>
#include <cpu.h>
#include <x86_64.h>
#include <pc.h>
#include <archdef.h>
#include <cboot.h>
#include <cpuid.h>
#include <debug/debug.h>
#include <memory/memoryarray.h>
#include <memorylayout.h>
#include <memory/heapmanager.h>
#include <interrupt/exceptions.h>
#include <interrupt/interrupts.h>
#include <gdt.h>
#include <tables/idt.h>
#include <memory/virtualmemorymanager.h>
#include <debug/lfbconsoleoutput.h>
#include <process/processmanager.h>
#include <process/userfunctions.h>


extern "C" uint8_t mmio;
extern "C" uint8_t kernelStart;
extern "C" uint8_t kernelEnd;

bool Cpu::IsBsp( void )
{
	return CurrentProcessorId() == cboot.bspId;
}

unsigned int Cpu::CurrentProcessorId()
{
	volatile uint32_t discard, ebx;
	__cpuid(1, discard, ebx, discard, discard);

	return ((ebx >> 24) & 0xFF);
}

namespace arch
{
	Spinlock X86_64::_pageLock;

	Gdt X86_64::_gdts[0x100];

	X86_64::X86_64()
	{
		_id = X86_64::CurrentProcessorId();

		VINFO("CPU ID is " << (uintptr_t)_id);

		if(Cpu::IsBsp())
		{
			//	2. Initialise IDT - we need to be able to handle interrupts
			//	   Installing the Double Fault handler makes the code a little more robust (preventing triple-faults?)
			VINFO("Initialise IDT");
			Idt::GetInstance().Load();
			Idt::GetInstance().InstallExceptionHandler(EXCEPTION_DOUBLE_FAULT, Exceptions::DoubleFaultExceptionHandler);
			Idt::GetInstance().InstallInterruptHandler(INTERRUPT_USER, Interrupts::SoftwareInterrupt);
			Idt::GetInstance().InstallExceptionHandler(EXCEPTION_PAGE_FAULT, Exceptions::PageFaultExceptionHandler);

			//	3. Initialise the virtual memory allocation system (BSP only).
			//	NOTE: Not complete until Tasking is also set up, due to load of new CR3
			INFO("Initialise Virtual Memory Manager");
			this->CreateKernelMemorySpace();
			VirtualMemoryManager::GetInstance().GetKernelAllocator().Initialise(HeapManager::RequestKernelHeapBytes);

			//	4. Set up an inital task and process block so that the arch-independent kernel
			//	can initialise ProcessManager. This is done *before* initialising the
			//	VMM as this will destroy mapping in the lower part of the heap manager.
			INFO("Creating initial Process and Thread");
			UserFunctions::GetInstance().Initialise(X86_64::SystemCall);
		}
		else
		{
			Gdt& gdt = _gdts[Cpu::CurrentProcessorId()];
			gdt.Load();

			Idt::GetInstance().Load();
			INFO("Attempt to initialise an non-BSP CPU.");
		}

		X86_64::EnableInterrupts();
	}

	void X86_64::CreateKernelMemorySpace( void )
	{
		
	}

	void X86_64::PageInLarge(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
	{
		uint64_t* pml4 = (uint64_t*)CPU_CLASS::ReadCr3();
		
		if(virtualAddress == UINT64_MAX) FATAL("Unable to page in - no physical memory allocated.");
		if((virtualAddress % 0x200000ULL) != 0) FATAL("Please align virtual address before paging in! At:" << virtualAddress);
		if((physicalAddress % 0x200000ULL) != 0) FATAL("Please align physical address before paging in! At:" << physicalAddress);

		_pageLock.Acquire();

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

		_pageLock.Release();
	}
}


