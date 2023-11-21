/**
 * @file idt.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains definitions associated with the IDT class.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_

#include <stdint.h>
#include <stddef.h>
#include <registers.h>
#include <debug.h>

namespace arch
{
	typedef void (*isr_t)( void );
	typedef Registers* (*InterruptHandler_t)(Registers*);

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

	//	software interrupt
	extern "C" void isr240( void );

	extern "C" void __loadIdt(void* idtr);
	extern "C" InterruptHandler_t __exception_call_table[0x20];

	/**
	 * @brief A singleton class which manages the interrupt descriptor table.
	 */
	class Idt
	{
	public:
		/// @brief Return the singleton instance of this class.
		static Idt& GetInstance( void ) { return _instance; }

		Idt( void );

		/// @brief Load the current IDT in to the current CPU.
		void Load( void );

		/// @brief Set up a specific entry in the IDT
		void SetEntry(uint8_t index, uint64_t address);
	public:
		/**
		 * @brief An entry in the Interrupt Desciptor Table
		 */
		struct IdtEntry
		{
		   uint16_t offsetLow;
		   uint16_t cs;
		   uint8_t  ist:4;
		   uint8_t 	reserved:4;
		   uint8_t	flags;
		   uint16_t offsetHigh;
		   uint32_t	offsetExtended;
		   uint32_t	reserved64;

		   IdtEntry() : offsetLow(0), cs(0), ist(0), reserved(0), flags(0), offsetHigh(0), offsetExtended(0), reserved64(0){}
		   IdtEntry(isr_t isr, uint16_t cs_, uint8_t flags_) : cs(cs_), reserved(0), flags(flags_), reserved64(0)
		   {
			   offsetLow = (uint16_t)(((uintptr_t)isr) & 0xFFFF);
			   offsetHigh = (uint16_t)((((uintptr_t)isr) >> 16) & 0xFFFF);
			   offsetExtended = (uint32_t)((((uintptr_t)isr) >> 32) & 0xFFFFFFFF);
		   }
		} __attribute__((packed));

		inline void InstallExceptionHandler(size_t index, InterruptHandler_t handler)
		{ 
			if(index >= 0x20) FATAL("Cannot install exception handler > index 31.");
			 __exception_call_table[index] = handler;
		}

		inline void InstallInterruptHandler(size_t index, InterruptHandler_t handler)
		{
			if(index < 0x80) FATAL("Cannot install a user interrupt handler < index 128.");
			__exception_call_table[index] = handler;
		}
	private:
		static Idt _instance;
		static Registers* DefaultHandler(Registers* regs);
	};
}

extern "C" arch::Idt::IdtEntry __idt[0x200];

#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_ */