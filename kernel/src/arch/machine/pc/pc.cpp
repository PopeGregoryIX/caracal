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

Machine& Machine::GetInstance( void ) { return arch::Pc::GetPcInstance(); }

namespace arch
{
	Pc Pc::_instance;

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
		
		//	4. Initialise the virtual memory allocation system
		Idt::GetInstance().InstallExceptionHandler(EXCEPTION_PAGE_FAULT, Exceptions::PageFaultExceptionHandler);
		char* x = (char*)0x100000000000ULL;
		x[1] = 'a';

		return true;
	}

}
