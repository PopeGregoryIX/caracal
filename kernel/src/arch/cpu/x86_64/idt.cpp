/*
 * idt.cpp
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#include <stdint.h>
#include <idt.h>

namespace arch
{
	Idt Idt::_instance;

	uint64_t* Idt::DefaultHandler(uint64_t* regs)
	{
		return regs;
	}
}

