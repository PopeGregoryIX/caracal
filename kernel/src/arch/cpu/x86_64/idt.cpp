/*
 * idt.cpp
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#include <stdint.h>
#include <idt.h>
#include <debug/debug.h>

namespace arch
{
	Idt Idt::_instance;

	void Idt::Initialise( void )
	{

	}

	uint64_t* Idt::DefaultHandler(uint64_t* regs)
	{
		FATAL("Unhandled Exception.");
		return regs;
	}
}

