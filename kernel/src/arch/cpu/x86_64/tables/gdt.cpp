/*
 * gdt.cpp
 *
 *  Created on: 8 Sep 2014
 *      Author: Adam
 */
#include <stdint.h>
#include <tables/gdt.h>

namespace arch
{
	Gdt Gdt::instance_;

	Gdt::Gdt( void )
	: entries_(0x2000)
	{
		//	NULL segment
		gdt_[0] = GdtEntry();

		//ring 0 segments
		gdt_[1].setBase(0);
		gdt_[1].setLimit(0xFFFFFFFF);
		gdt_[1].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[1].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW;

		gdt_[2].setBase(0);
		gdt_[2].setLimit(0xFFFFFFFF);
		gdt_[2].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[2].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW;

		//ring 3 segments
		gdt_[3].setBase(0);
		gdt_[3].setLimit(0xFFFFFFFF);
		gdt_[3].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[3].access = GDT_PRESENT | GDT_SYSTEM | GDT_EXECUTABLE | GDT_RW | GDT_USER;

		gdt_[4].setBase(0);
		gdt_[4].setLimit(0xFFFFFFFF);
		gdt_[4].flags = GDT_GRAN_4K | GDT_SIZE64;
		gdt_[4].access = GDT_PRESENT | GDT_SYSTEM | GDT_RW | GDT_USER;
	}

	void Gdt::Load( void )
	{
		gdtr_.size = (uint16_t)((entries_ * sizeof(GdtEntry)) - 1);
		gdtr_.offset = (uintptr_t)gdt_;

		__loadGdt((void*)&gdtr_, 0x08, 0x10);
	}
}



