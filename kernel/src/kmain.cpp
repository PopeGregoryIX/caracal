#include <stdint.h>
#include <bootboot.h>
#include <debug/debug.h>
#include <runtime/cxx.h>
#include <machine.h>
#include <cpu.h>

extern "C" void kmain();
extern "C" void _init( void );
extern "C" void _fini( void );

void kmain()
{
	//	At this point we are limited to statics (no heap allocation)
	//	kmain is called by *all* SMP cores and therefore needs to
	//	distinguish between BSP's and AP's early on in code
	_init();

	if(Cpu::IsBsp())
	{
		//	The aim here is to spend a relatively short time in this kmain
		//	function. In the style of a microkernel, we want to exit this
		//	boot process and get everything working inside its own process
		//	space. This means AP's will also take part in system
		//	initialisation.
		DebugConsole& debug = DebugConsole::GetInstance();
		Machine::GetInstance().AddDefaultConsoleDevices(debug);

		debug << ConsoleColour(0xFFFFFF, 0x000000);
		INFO( "Caracal v1.0 Debug Console");
		INFO( "BSP ID: " << (uint64_t)Cpu::ProcessorId());

		if(Machine::GetInstance().Boot())
		{
			INFO("Boot routine complete...");
		}
		else
			FATAL("Boot routine failed...");
	}

	while(1);

	_fini();
}
