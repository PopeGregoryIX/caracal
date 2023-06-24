/*
 * tss.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#include <tables/tss.h>
#include <tables/gdt.h>

namespace arch
{
	Tss Tss::instance_;

	void Tss::InstallToGdt(int tssEntryNumber, int gdtEntry)
	{
		Gdt::GdtSystemEntry entry;
		entry.setBase((uintptr_t)(&Tss::__tss[tssEntryNumber]));
		entry.setLimit(sizeof(entry));
		entry.access = 0x89;
		entry.flags = 0x4;

		Gdt::GetInstance().SetEntry(gdtEntry, entry);
	}

	void Tss::Load(int gdtEntryNumber)
	{
		gdtEntryNumber -= 6;
		gdtEntryNumber = 0x30 + (gdtEntryNumber * 0x10);
		asm volatile ( "ltr %0" : : "m"(gdtEntryNumber) );
	}
}


