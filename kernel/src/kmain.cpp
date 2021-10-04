/**
 * @file kmain.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains the multi-platform kernel entry point (use the BOOTBOOT bootloader).
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <bootboot.h>
#include <debug/debug.h>
#include <runtime/cxx.h>
#include <machine.h>
#include <cpu.h>
#include <process/process.h>

extern "C" void kmain();
extern "C" void _init( void );
extern "C" void _fini( void );

/**
 * @brief Kernel entry point.
 * 
 * Multi-platform entry point as loaded by the BOOTBOOT loader. This is 
 * a well-defined protocol for each platform. Platform-dependent code in
 * Machine and arch is called to perform the actual boot. Note
 * that on an SMP system, all cores may call kmain at the same time.
 * To port to another platform:
 * 	- Implement the Machine object.
 * 	- Implement archdef.h.
 */
void kmain()
{
	//	At this point we are limited to statics (no heap allocation)
	//	kmain is called by *all* SMP cores and therefore needs to
	//	distinguish between BSP's and AP's early on in code
	if(Cpu::IsBsp())
	{
		_init();

		//	The aim here is to spend a relatively short time in this kmain
		//	function. In the style of a microkernel, we want to exit this
		//	boot process and get everything working inside its own process
		//	space. This means AP's will also take part in system
		//	initialisation.
		DebugConsole& debug = DebugConsole::GetInstance();
		Machine::GetInstance().AddDefaultConsoleDevices(debug);

		debug << ConsoleColour(0xFFFFFF, 0x000000);
		INFO( "Caracal v1.0 Debug Console" );
		INFO( "BSP ID: " << (uint64_t)Cpu::ProcessorId());
	
		if(Machine::GetInstance().Boot())
		{
			INFO("Architecture-specific boot routine complete");
		}
		else
		{
			FATAL("Boot routine failed");
		}

		char* x = new char[0x200];
		INFO("Test Memory Allocation at " << (uintptr_t)x);
		x[0] = 0;
		delete x;
	}
	else
		Machine::GetInstance().HaltCurrentCore();

	FATAL("Reached end of kmain routine. No further code to execute");

	_fini();
}
