/*
 * tss.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#include <tss.h>
#include <gdt.h>

namespace arch
{

	/*void Tss::Load(int gdtEntryNumber)
	{
		gdtEntryNumber -= 5;
		gdtEntryNumber = 0x30 + (gdtEntryNumber * 0x10);
		asm volatile ( "ltr %0" : : "m"(gdtEntryNumber) );
	}*/
}


