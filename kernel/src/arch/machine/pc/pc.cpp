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


		//	1. Initialise IDT - we need to be able to handle interrupts
		INFO("Initialise IDT on BSP");

		//	2. Initialise the memory map - we need to be able to allocate physical pages
		INFO("Initialise Page Frame Allocator");
		return false;
	}

}
