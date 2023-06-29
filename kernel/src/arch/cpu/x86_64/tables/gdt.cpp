/*
 * gdt.cpp
 *
 *  Created on: 8 Sep 2014
 *      Author: Adam
 */
#include <stdint.h>
#include <tables/gdt.h>
#include <runtime/cxx.h>
#include <debug/debug.h>

namespace arch
{
	Gdt Gdt::instance_;

	Gdt::Gdt( void )
	: entries_(0x2000)
	{
		//	NULL segment
		gdt_[GDTI_NULL] = GdtEntry();

		//ring 0 segments 0x08
		gdt_[GDTI_RING0_CODE].setBase(0);
		gdt_[GDTI_RING0_CODE].setLimit(0xFFFFFFFF);
		gdt_[GDTI_RING0_CODE].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[GDTI_RING0_CODE].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW;

		gdt_[GDTI_RING0_DATA].setBase(0);
		gdt_[GDTI_RING0_DATA].setLimit(0xFFFFFFFF);
		gdt_[GDTI_RING0_DATA].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[GDTI_RING0_DATA].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW;

		//ring 3 segments 0x18
		gdt_[GDTI_RING3_CODE].setBase(0);
		gdt_[GDTI_RING3_CODE].setLimit(0xFFFFFFFF);
		gdt_[GDTI_RING3_CODE].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[GDTI_RING3_CODE].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW | GDT_USER;

		gdt_[GDTI_RING3_DATA].setBase(0);
		gdt_[GDTI_RING3_DATA].setLimit(0xFFFFFFFF);
		gdt_[GDTI_RING3_DATA].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[GDTI_RING3_DATA].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW | GDT_USER;
	}



	void Gdt::Load( void )
	{
		gdtr_.size = (uint16_t)((entries_ * sizeof(GdtEntry)) - 1);
		gdtr_.offset = (uintptr_t)gdt_;

		__loadGdt((void*)&gdtr_, 0x08, 0x10);
	}

	void Gdt::SetEntry(int index, GdtSystemEntry entry)
	{
		if(index < 5) FATAL("Attempt to set a core GDT descriptor after initialisation.");

		GdtSystemEntry* gdtSys = (GdtSystemEntry*)&gdt_[index];
		memorycopy<GdtSystemEntry>(gdtSys, &entry, 1);
	}
}



