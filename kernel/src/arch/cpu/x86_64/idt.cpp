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
		_idt[0] = IdtEntry(isr0, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[1] = IdtEntry(isr1, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[2] = IdtEntry(isr2, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[3] = IdtEntry(isr3, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[4] = IdtEntry(isr4, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[5] = IdtEntry(isr5, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[6] = IdtEntry(isr6, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[7] = IdtEntry(isr7, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[8] = IdtEntry(isr8, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[9] = IdtEntry(isr9, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[10] = IdtEntry(isr10, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[11] = IdtEntry(isr11, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[12] = IdtEntry(isr12, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[13] = IdtEntry(isr13, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[14] = IdtEntry(isr14, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[15] = IdtEntry(isr15, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[16] = IdtEntry(isr16, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[17] = IdtEntry(isr17, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[18] = IdtEntry(isr18, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[19] = IdtEntry(isr19, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[20] = IdtEntry(isr20, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[21] = IdtEntry(isr21, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[22] = IdtEntry(isr22, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[23] = IdtEntry(isr23, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[24] = IdtEntry(isr24, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[25] = IdtEntry(isr25, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[26] = IdtEntry(isr26, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[27] = IdtEntry(isr27, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[28] = IdtEntry(isr28, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[29] = IdtEntry(isr29, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[30] = IdtEntry(isr30, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
		_idt[31] = IdtEntry(isr31, 0x08, IDT_GATE_INTERRUPT | IDT_GATE_PRESENT);
	}

	void Idt::Load()
	{
		_idtr.size = (uint16_t)((0x200 * sizeof(IdtEntry)) - 1);
		_idtr.offset = (uintptr_t)_idt;

		__loadIdt((void*)&_idtr);
	}

	Registers* Idt::DefaultHandler(Registers* regs)
	{
		FATAL("Unhandled Exception.");
		return regs;
	}
}

