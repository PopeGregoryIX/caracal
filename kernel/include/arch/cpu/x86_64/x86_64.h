/**
 * @file x86_64.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains the X86_64 version of the Cpu class.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
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
