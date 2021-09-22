/*
 * pc.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <machine.h>
#include <pc.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>

Machine& Machine::GetInstance( void ) { return arch::Pc::GetPcInstance(); }

namespace arch
{
	Pc Pc::_instance;

	void Pc::AddDefaultConsoleDevices(DebugConsole& console)
	{
		console.AddOutputDevice(LfbConsoleOutput::GetInstance());
	}

}
