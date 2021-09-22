#include <stdint.h>
#include <bootboot.h>
#include <pcscreenfont.h>
#include <debug/debugconsole.h>
#include <runtime/cxx.h>
#include <string.h>
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
		DebugConsole& debug = DebugConsole::GetInstance();
		Machine::GetInstance().AddDefaultConsoleDevices(debug);

		debug << ConsoleColour(0xFFFFFF, 0x000000);
		debug << "Caracal v1.0 Debug Console\n";
		debug << "BSP ID: " << (uint64_t)Cpu::ProcessorId() << "\n";
	}

	while(1);

	_fini();
}
