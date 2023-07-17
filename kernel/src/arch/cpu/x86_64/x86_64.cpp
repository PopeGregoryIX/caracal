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
#include <bootboot.h>
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
	return CurrentProcessorId() == bootboot.bspid;
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
			//	1. Initialise GDT - it's in an unknown state
			VINFO("Initialise GDT");
			_gdts[bootboot.bspid].Load();
			_gdts[bootboot.bspid].LoadTss();

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
			if(Cpu::CurrentProcessorId() > bootboot.numcores) FATAL("Some assumptions are made about CPU core numbers, but they are not sequential!");
			Gdt& gdt = _gdts[Cpu::CurrentProcessorId()];
			gdt.Load();

			Idt::GetInstance().Load();
			INFO("Attempt to initialise an non-BSP CPU.");
		}

		X86_64::EnableInterrupts();
	}

	void X86_64::CreateKernelMemorySpace( void )
	{
		INFO("Creating paging structures");

		pageDirectoryEntry_t* pml4 = (pageDirectoryEntry_t*)CPU_CLASS::ReadCr3();

		//	First, identity map the entire physical page space
		uint64_t maxPhysical = MemoryArray::GetInstance().GetHighestAddress();
		uint64_t pageDirectoryCount = maxPhysical / PD_RANGE;
		if(maxPhysical % PD_RANGE) pageDirectoryCount++;

		pageDirectoryEntry_t* tmpPdpt = (pageDirectoryEntry_t*)(pml4[0].Address());
		pageDirectoryEntry_t* tmpPd;

		//	BootBoot maps the lower 16GiB. Remap with large pages
		if(maxPhysical > (16ULL * 1024ULL * 1024ULL * 1024ULL)) 
		{
			FATAL("Physical memory size > 16GiB. CreateInitialProcessSpace needs to be amended.");
			maxPhysical = (16ULL * 1024ULL * 1024ULL * 1024ULL);
		}

		for(size_t i = 0; i <= maxPhysical; i += PDE_RANGE)
		{ 
			if((i % PDPT_RANGE) == 0)
				tmpPd = (pageDirectoryEntry_t*)tmpPdpt[i / PD_RANGE].Address();

			((uint64_t*)tmpPd)[(i % PD_RANGE) / PDE_RANGE] = i | PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL;
		}
		
		tmpPd = (pageDirectoryEntry_t*)(tmpPdpt[0]).Address();
		((uint64_t*)tmpPd)[0] = 0; // < To catch null references

		/**
		 * Based on the bootboot specification, we have MMIO then the LFB,
		 * then the BOOTBOOT sructure.
		 *
		 */		

		//	1 - Copy the entire kernel in to a new space aligned at 2MiB (the additional 0x2000 is for BootBoot structures)
		uintptr_t kernelSize = ((uint64_t)&kernelEnd - (uint64_t)&kernelStart) + 0x2000;
		uintptr_t kernelPages = kernelSize / 0x200000;
		if(kernelSize % 0x200000) kernelPages++;
		uintptr_t newKernel = PageFrameAllocator::GetInstance().Allocate(kernelPages * 0x200000, 0x200000);
		if(newKernel == 0xFFFFFFFFFFFFFFFFULL) FATAL("Unable to allocate physical memory for Kernel.");
		::memorycopy((void*)newKernel, (void*)((uint64_t)&kernelStart - 0x2000), kernelSize);

		tmpPdpt = (pageDirectoryEntry_t*)pml4[0x1FF].Address();
		tmpPd = (pageDirectoryEntry_t*)tmpPdpt[0x1FF].Address();
				
		/**
		 * Iterate over the kernel PD, unmapping all unused pages
		 * TODO: We need to map out the MMIO
		 * 
		 * The LFB should only need 2MiB max (this even covers 4k @ 32 bit).
		 */
		uintptr_t pdBase = (0xFFFFFFFFFFFFFFFF - PD_RANGE) + 1;
		uintptr_t lfbLoc = tmpPd[PD_INDEX((size_t)&fb)].Address();

		for(size_t i = pdBase; i < UINT64_MAX - 0x200000; i+= 0x200000)
		{
			if(i >= (uint64_t)&fb && i <= ((uint64_t)&fb + 0x200000))
				((uint64_t*)tmpPd)[PD_INDEX(i)] = (lfbLoc + (i - (uint64_t)&fb)) 
										| PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL;
			else if((i >= ((uint64_t)&kernelStart - 0x2000)) && i <= (uint64_t)&kernelEnd)
				((uint64_t*)tmpPd)[PD_INDEX(i)] = (newKernel + (i - (uint64_t)(&kernelStart - 0x2000))) 
										| PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL;
			else
				((uint64_t*)tmpPd)[PD_INDEX(i)] = 0;
		}

		/**
		 * 	Next, clear all PML4 entries except 0 (identity mapping of physical RAM,
		 *  0x1FE (page structures) and 0x1FF (kernel space)
		 */		
		for(size_t i = 1; i < 0x1FF; ++i) ((uint64_t*)pml4)[i] = 0;

		//	Create a PDPT for the kernel heap
		((uint64_t*)pml4)[0x1FE] = PageFrameAllocator::GetInstance().Allocate() | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL;
		CPU_CLASS::WriteCr3((uint64_t)pml4);
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


