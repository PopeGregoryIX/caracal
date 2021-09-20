#include <stdint.h>
#include <bootboot.h>
#include <pcscreenfont.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <runtime/cxx.h>
#include <string.h>

extern "C" void kmain();

void kmain()
{
	//	At this point we are limited to statics (no heap allocation)
	//::callConstructors();
	DebugConsole& debug = DebugConsole::GetInstance();
	debug.AddOutputDevice(arch::LfbConsoleOutput::GetInstance());

	//debug.PutString("Booting Caracal v0.1...");
	//arch::LfbConsoleOutput::GetInstance().PutString("String");
	typedef void (*function_pointer) (void);

	extern function_pointer start_ctors[];
    extern function_pointer end_ctors[];

    char buf[64];
    ::String::itoa((size_t)start_ctors, buf);
    buf[16] = '-';
    ::String::itoa((size_t)end_ctors, &(buf[17]));
	arch::LfbConsoleOutput cout;
	cout.PutString(buf);

    //  Note that kmain is called by ALL SMP cores!

    //  TODO: write local APIC driver and determine which is the BSP
    /*  i.e.
     *  if (currentcoreid == bootboot.bspid)
     *  {
     *      do boot things
     *  }
     *  else
     *  {
     *      do AP things...
     *  }
     */

	while(1);
}
