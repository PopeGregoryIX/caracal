/*
 * pc.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <machine.h>
#include <pc.h>
#include <runtime/cxx.h>
#include <x86_64.h>
#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include <debug/lfbconsoleoutput.h>
#include <tables/gdt.h>
#include <tables/idt.h>
#include <memory/pageframeallocator.h>
#include <interrupt/exceptions.h>
#include <process/process.h>
#include <process/processmanager.h>
#include <process/thread.h>
#include <memory/memoryarray.h>
#include <memorylayout.h>

Machine& Machine::GetInstance( void ) { return arch::Pc::GetPcInstance(); }
extern "C" uint8_t mmio;
extern "C" uint8_t kernelStart;
extern "C" uint8_t kernelEnd;

namespace arch
{
	Pc Pc::_instance;
	Process Pc::_initialProcess(0, nullptr);
	Thread Pc::_initialThread(0, nullptr);

	void Pc::AddDefaultConsoleDevices(DebugConsole& console)
	{
		console.AddOutputDevice(LfbConsoleOutput::GetInstance());
	}

	bool Pc::Boot( void )
	{
		//	This is where we set up the machine. This is purely called
		//	by the BSP.
		INFO("Running boot routines for machine type: PC");

		//	1. Initialise GDT - it's in an unknown state
		INFO("Initialise GDT on BSP");
		Gdt::GetInstance().Load();

		//	2. Initialise IDT - we need to be able to handle interrupts
		INFO("Initialise IDT on BSP");
		Idt::GetInstance().Load();

		//	3. Initialise the memory map - we need to be able to allocate physical pages
		INFO("Initialise Page Frame Allocator");
		PageFrameAllocator& pageFrameAllocator = ::PageFrameAllocator::GetInstance();
		pageFrameAllocator.Initialise(0x1000);
		
		//	4. Initialise the virtual memory allocation system.
		//	NOTE: Not complete until Tasking is also set up, due to load of new CR3
		INFO("Initialise Virtual Memory Manager");
		Idt::GetInstance().InstallExceptionHandler(EXCEPTION_PAGE_FAULT, Exceptions::PageFaultExceptionHandler);

		//	5. Set up an inital task and process block so that the arch-independent kernel
		//	can initialise ProcessManager.
		INFO("Creating initial Process and Thread");
		this->CreateInitialProcessSpace();
		return true;
	}

	void Pc::CreateInitialProcessSpace( void )
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

		/**
		 * Based on the bootboot specification, we have MMIO then the LFB,
		 * then the BOOTBOOT sructure.
		 * The LFB should only need 2MiB max (this even covers 4k @ 32 bit).
		 *
		 * Now map the higher half.
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

		for(size_t i = 0; i < kernelPages; ++i)
			((uint64_t*)tmpPd)[PD_INDEX((uintptr_t)&kernelStart) + i] = (newKernel + (i * 0x200000)) | PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE | PAGE_GLOBAL;		
		
		//	next, blank all memory between the end of the kernel and the stack, leaving just the single 2MiB page at the top of RAM for the stack
		CPU_CLASS::WriteCr3((uint64_t)pml4);
	}

}
