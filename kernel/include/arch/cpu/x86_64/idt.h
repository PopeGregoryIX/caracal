/*
 * idt.h
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_

#include <stdint.h>
#include <registers.h>

typedef void (*isr_t)( void );
extern "C" void __loadIdt(void* idtr);

#define IDT_GATE_TASK		0x05
#define IDT_GATE_INTERRUPT	0x0E
#define IDT_GATE_TRAP		0x0F
#define IDT_GATE_STORAGE	(0x01 << 4)
#define IDT_GATE_USER		(0x03 << 5)
#define IDT_GATE_PRESENT	(0x01 << 7)

extern "C" void isr0( void );
extern "C" void isr1( void );
extern "C" void isr2( void );
extern "C" void isr3( void );
extern "C" void isr4( void );
extern "C" void isr5( void );
extern "C" void isr6( void );
extern "C" void isr7( void );
extern "C" void isr8( void );
extern "C" void isr9( void );
extern "C" void isr10( void );
extern "C" void isr11( void );
extern "C" void isr12( void );
extern "C" void isr13( void );
extern "C" void isr14( void );
extern "C" void isr15( void );
extern "C" void isr16( void );
extern "C" void isr17( void );
extern "C" void isr18( void );
extern "C" void isr19( void );
extern "C" void isr20( void );
extern "C" void isr21( void );
extern "C" void isr22( void );
extern "C" void isr23( void );
extern "C" void isr24( void );
extern "C" void isr25( void );
extern "C" void isr26( void );
extern "C" void isr27( void );
extern "C" void isr28( void );
extern "C" void isr29( void );
extern "C" void isr30( void );
extern "C" void isr31( void );

namespace arch
{
	class Idt
	{
	public:
		static Idt& GetInstance( void ) { return _instance; }

		void Initialise( void );

		void Load( void );

		void SetEntry(uint8_t index, uint64_t address);
	public:
		struct IdtEntry
		{
		   uint16_t offsetLow;
		   uint16_t cs;
		   uint8_t 	reserved;
		   uint8_t	flags;
		   uint16_t offsetHigh;
		   uint32_t	offsetExtended;

		   uint32_t	reserved64;
		   IdtEntry() : offsetLow(0), cs(0), reserved(0), flags(0), offsetHigh(0), offsetExtended(0), reserved64(0){}
		   IdtEntry(isr_t isr, uint16_t cs_, uint8_t flags_) : cs(cs_), reserved(0), flags(flags_), reserved64(0)
		   {
			   offsetLow = (uint16_t)(((uintptr_t)isr) & 0xFFFF);
			   offsetHigh = (uint16_t)((((uintptr_t)isr) >> 16) & 0xFFFF);
			   offsetExtended = (uint32_t)((((uintptr_t)isr) >> 32) & 0xFFFFFFFF);
		   }
		} __attribute__((packed));

		struct IdtDescriptor
		{
			uint16_t size;
			uintptr_t offset;
		} __attribute__((packed));
	private:
		static Idt _instance;

		Registers* DefaultHandler(Registers* regs);

		IdtEntry _idt[0x200];
		IdtDescriptor _idtr;
	};
}

extern "C" arch::Idt::IdtEntry __idt[256];

#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_ */
