/*
 * x86_64.h
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_X86_64_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_X86_64_H_

#include <cpu.h>

namespace arch
{
	class X86_64 : public Cpu
	{

	};
}

typedef arch::X86_64 CPU;

#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_X86_64_H_ */
