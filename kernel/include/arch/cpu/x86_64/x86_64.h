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
		public:
			static inline uint8_t In8(uint16_t port)	{ uint8_t value; asm volatile ( "inb %1, %0" : "=a"(value) : "Nd"(port) ); return value; }
			static inline uint16_t In16(uint16_t port)	{ uint16_t value; asm volatile ( "inw %1, %0" : "=a"(value) : "Nd"(port) ); return value; }
			static inline uint32_t In32(uint16_t port)	{ uint32_t value; asm volatile ( "inl %1, %0" : "=a"(value) : "Nd"(port) ); return value; }

			static inline void Out8(uint16_t port, uint8_t value)	{ asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) );	}
			static inline void Out16(uint16_t port, uint16_t value)	{ asm volatile ( "outw %0, %1" : : "a"(value), "Nd"(port) );	}
			static inline void Out32(uint16_t port, uint32_t value)	{ asm volatile ( "outl %0, %1" : : "a"(value), "Nd"(port) );	}

			static inline uint64_t ReadCr0(void) { uint64_t value; asm volatile ( "movq %%cr0, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr2(void) { uint64_t value; asm volatile ( "movq %%cr2, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr3(void) { uint64_t value; asm volatile ( "movq %%cr3, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr4(void) { uint64_t value; asm volatile ( "movq %%cr4, %0" : "=r"(value) ); return value; }

			static inline void WriteCr3(uint64_t value) { asm volatile ( "movq %0, %%cr3" : : "a"(value) ); }
			static inline void WriteCr4(uint64_t value) { asm volatile ( "movq %0, %%cr4" : : "a"(value) ); }
	};
}

typedef arch::X86_64 CPU;

#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_X86_64_H_ */
