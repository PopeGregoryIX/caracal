/**
 * \file registers.h
 * \author Adam Jones
 * \copyright (c) 2014 Adam Jones
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

namespace arch
{
	struct Registers
	{
	   uint64_t	ds;
	   uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	   uint64_t rsi, rdi, rsp, rbp, rdx, rcx, rbx, rax;
	   uint64_t interruptNumber, errorCode;
	   uint64_t rip, cs, rflags, userRsp, userSs;
	}__attribute__((packed));
}



#endif /* REGISTERS_H_ */
