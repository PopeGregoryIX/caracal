/*
 * idt.cpp
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#include <stdint.h>
#include <tables/idt.h>
#include <debug/debug.h>

namespace arch
{
	Idt Idt::_instance;

	Idt::Idt( void )
	{
		__idt[0] = IdtEntry(isr0, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[1] = IdtEntry(isr1, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[2] = IdtEntry(isr2, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[3] = IdtEntry(isr3, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[4] = IdtEntry(isr4, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[5] = IdtEntry(isr5, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[6] = IdtEntry(isr6, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[7] = IdtEntry(isr7, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[8] = IdtEntry(isr8, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[9] = IdtEntry(isr9, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[10] = IdtEntry(isr10, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[11] = IdtEntry(isr11, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[12] = IdtEntry(isr12, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[13] = IdtEntry(isr13, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[14] = IdtEntry(isr14, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[15] = IdtEntry(isr15, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[16] = IdtEntry(isr16, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[17] = IdtEntry(isr17, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[18] = IdtEntry(isr18, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[19] = IdtEntry(isr19, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[20] = IdtEntry(isr20, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[21] = IdtEntry(isr21, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[22] = IdtEntry(isr22, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[23] = IdtEntry(isr23, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[24] = IdtEntry(isr24, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[25] = IdtEntry(isr25, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[26] = IdtEntry(isr26, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[27] = IdtEntry(isr27, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[28] = IdtEntry(isr28, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[29] = IdtEntry(isr29, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[30] = IdtEntry(isr30, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		__idt[31] = IdtEntry(isr31, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);

		__idt[0xF0] = IdtEntry(isr240, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
	}

	void Idt::Load()
	{
		struct 
		{
			uint16_t length;
			void*    base;
		} __attribute__((packed)) IDTR = { 0x200 * sizeof(struct IdtEntry), (void*)__idt };
 
    	asm ( "lidt %0" : : "m"(IDTR) );
	}

	Registers* Idt::DefaultHandler(Registers* regs)
	{
		FATAL("Unhandled interrupt which has not been claimed by Exception, IRQ or System Interrupt handler.");
		return regs;
	}
}

