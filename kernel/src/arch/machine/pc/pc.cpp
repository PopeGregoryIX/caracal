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
#include <gdt.h>
#include <idt.h>

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
		Gdt::GetInstance().Load();

		//	1. Initialise GDT - it's in an unknown state
		INFO("Initialise GDT on BSP");
		Idt::GetInstance().Load();
		int x = 0;
		x = 1 / x;

		//	2. Initialise IDT - we need to be able to handle interrupts
		INFO("Initialise IDT on BSP");

		//	3. Initialise the memory map - we need to be able to allocate physical pages
		INFO("Initialise Page Frame Allocator");
		return false;
	}

}
