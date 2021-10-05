/**
 * @file registers.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief X86-64 registers saved on the stack during an interrupt.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

namespace arch
{
	/**
	 * @brief X86-64 general-purpose registers plus the ISR call stack.
	 */
	struct Registers
	{
	   uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	   uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	   uint64_t interruptNumber, errorCode;
	   uint64_t rip, cs, rflags, userRsp, userSs;

	   uint64_t* Stack(){ return (uint64_t*)(((uint64_t)this) + sizeof(struct Registers)); }
	}__attribute__((packed));
}

#endif /* REGISTERS_H_ */
