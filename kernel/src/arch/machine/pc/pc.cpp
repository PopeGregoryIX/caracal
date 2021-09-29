/*
 * pc.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <machine.h>
#include <pc.h>
#include <x86_64.h>
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
		INFO("PML4: " << (uint64_t)pml4);

		//	First, identity map the entire physical page space
		uint64_t maxPhysical = MemoryArray::GetInstance().GetHighestAddress();
		uint64_t pageDirectoryCount = maxPhysical / PD_RANGE;

		INFO("Page directories required = " << pageDirectoryCount);

		/**
		 * We can use a 2MiB window at 0x200000 - 0x400000 to prepare paging structures
		 * to create all the various paging structures we need.
		 * To aid this, we allocate all page directories on a 2MiB boundary.
		 */
		pageDirectoryEntry_t* lowPdpt = (pageDirectoryEntry_t*)(pml4[0].Address());
		pageDirectoryEntry_t* lowPd = (pageDirectoryEntry_t*)(lowPdpt[0].Address());

		//	BootBoot maps the lower 16GiB. Remap with large tables
		for(size_t i = 0; i < PD_RANGE; i += PDE_RANGE) ((uint64_t*)lowPd)[i / PDE_RANGE] = i | 0x103ULL;
		CPU_CLASS::WriteCr3((uint64_t)pml4);
	}

}
