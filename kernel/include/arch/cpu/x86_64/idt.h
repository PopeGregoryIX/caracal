/*
 * idt.h
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_

#include <stdint.h>

namespace arch
{
	class Idt
	{
	public:
		static Idt& GetInstance( void ) { return _instance; }

		void Initialise( void );
	private:
		static Idt _instance;

		uint64_t* DefaultHandler(uint64_t* regs);

		struct Entry
		{
			uint16_t 	LowAddress;
			uint16_t 	Selector;
			uint8_t		IstOffset;
			uint8_t		Type;
			uint16_t	MidAddress;
			uint32_t	HighAddress;
			uint32_t	Reserved;
		};
	};
}

#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_IDT_H_ */
