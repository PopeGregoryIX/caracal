/*
 * gdt.cpp
 *
 *  Created on: 8 Sep 2014
 *      Author: Adam
 */
#include <stdint.h>
#include <gdt.h>
#include <cxx.h>
#include <debug.h>
#include <memorylayout.h>

namespace arch
{
	Gdt::Gdt( void ){ Initialise(); }

	void Gdt::Initialise( void )
	{
		//	NULL segment
		_gdt[GDTI_NULL] = GdtEntry();

		// ring 0 segments
		_gdt[GDTI_RING0_CODE].setBase(0);
		_gdt[GDTI_RING0_CODE].setLimit(0xFFFFFFFF);
		_gdt[GDTI_RING0_CODE].flags = GDT_GRAN_4K | GDT_SIZE64;
		_gdt[GDTI_RING0_CODE].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW;

		_gdt[GDTI_RING0_DATA].setBase(0);
		_gdt[GDTI_RING0_DATA].setLimit(0xFFFFFFFF);
		_gdt[GDTI_RING0_DATA].flags = GDT_GRAN_4K | GDT_SIZE64;
		_gdt[GDTI_RING0_DATA].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW;

		// 64 bit ring 3 segments
		_gdt[GDTI_RING3_CODE].setBase(0);
		_gdt[GDTI_RING3_CODE].setLimit(0xFFFFFFFF);
		_gdt[GDTI_RING3_CODE].flags = GDT_GRAN_4K | GDT_SIZE64;
		_gdt[GDTI_RING3_CODE].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW | GDT_USER;

		_gdt[GDTI_RING3_DATA].setBase(0);
		_gdt[GDTI_RING3_DATA].setLimit(0xFFFFFFFF);
		_gdt[GDTI_RING3_DATA].flags = GDT_GRAN_4K | GDT_SIZE64;
		_gdt[GDTI_RING3_DATA].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW | GDT_USER;

		//	tss
		Gdt::GdtSystemEntry tssEntry;
		tssEntry.setBase((uintptr_t)(&_tss));
		tssEntry.setLimit(sizeof(arch::Tss));
		tssEntry.access = 0x89;
		tssEntry.flags = 0x4;
		
		
		//	Enable a TSS entry specifically for a stack for handling double-faults
		//_tss.ist1_high = (MEMRANGE_DFE_STACK_TOP > 32) & 0xFFFFFFFFULL;
		//_tss.ist1_low = MEMRANGE_DFE_STACK_TOP & 0x00000000FFFFFFFFULL;

		GdtSystemEntry* gdtSys = (GdtSystemEntry*)&_gdt[GDTI_TSS];
		memorycopy<GdtSystemEntry>(gdtSys, &tssEntry, 1);
	}

	void Gdt::Load( void )
	{
		_gdtr.size = (uint16_t)((GDT_ENTRY_COUNT * sizeof(GdtEntry)) - 1);
		_gdtr.offset = (uintptr_t)_gdt;

		__loadGdt((void*)&_gdtr, GDTE_RING0_CODE, GDTE_RING0_DATA);
	}

	void Gdt::LoadTss( void )
	{
		int tssEntry = GDTE_TSS;
		asm volatile ( "ltr %0" : : "m"(tssEntry) );
	}
}



