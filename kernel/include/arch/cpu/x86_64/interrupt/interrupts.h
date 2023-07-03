/*
 * interrupts.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_INTERRUPT_INTERRUPTS_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_INTERRUPT_INTERRUPTS_H_

#include <stdint.h>
#include <stddef.h>
#include <tables/idt.h>

namespace arch
{
	#define INTERRUPT_USER	240

	class Interrupts
	{
		public:
			/**
			 * The software interrupt service - used when SYSCALL / SYSENTER not available.
			 * @param registers CPU state as saved by the interrupt handler.
			 * @return New register state following the interrupt.
			 * 
			*/
			static Registers* SoftwareInterrupt(Registers* registers);
	};
}


#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_INTERRUPT_INTERRUPTS_H_ */
