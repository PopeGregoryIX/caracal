#ifndef __X86__64__H__
#define __X86__64__H__

#include <stdint.h>
#include <stddef.h>

#define MSR_EFER	0xC0000080
#define MSR_STAR	0xC0000081
#define MSR_LSTAR 	0xC0000082
#define MSR_SFMASK 	0xC0000084

namespace arch
{
	extern "C" void __stackReset(uintptr_t stackPtr, uintptr_t jumpPoint, uintptr_t jmpparams);
	extern "C" void __systemCall();

    #define INTERRUPT_USER 0xF0

    class X86_64_Utilities
    {
        public:
            static inline void EnableInterrupts( void ) { asm volatile("sti"); }
			static inline void DisableInterrupts( void ) { asm volatile("cli"); }

			static inline uint8_t In8(uint16_t port)	{ uint8_t value; asm volatile ( "inb %1, %0" : "=a"(value) : "Nd"(port) ); return value; }
			static inline uint16_t In16(uint16_t port)	{ uint16_t value; asm volatile ( "inw %1, %0" : "=a"(value) : "Nd"(port) ); return value; }
			static inline uint32_t In32(uint16_t port)	{ uint32_t value; asm volatile ( "inl %1, %0" : "=a"(value) : "Nd"(port) ); return value; }

			static inline void InvalidatePage(unsigned long address)	{	asm volatile("invlpg (%0)" ::"r" (address) : "memory"); }

			static inline void Out8(uint16_t port, uint8_t value)	{ asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) );	}
			static inline void Out16(uint16_t port, uint16_t value)	{ asm volatile ( "outw %0, %1" : : "a"(value), "Nd"(port) );	}
			static inline void Out32(uint16_t port, uint32_t value)	{ asm volatile ( "outl %0, %1" : : "a"(value), "Nd"(port) );	}

			static inline uint64_t ReadCr0(void) { uint64_t value; asm volatile ( "movq %%cr0, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr2(void) { uint64_t value; asm volatile ( "movq %%cr2, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr3(void) { uint64_t value; asm volatile ( "movq %%cr3, %0" : "=r"(value) ); return value; }
			static inline uint64_t ReadCr4(void) { uint64_t value; asm volatile ( "movq %%cr4, %0" : "=r"(value) ); return value; }

			static inline void WriteCr3(uint64_t value) { asm volatile ( "movq %0, %%cr3" : : "a"(value) ); }
			static inline void WriteCr4(uint64_t value) { asm volatile ( "movq %0, %%cr4" : : "a"(value) ); }

			static inline uint64_t ReadMsr(uint64_t msr)
			{			
				uint32_t low, high;
				asm volatile (
				"rdmsr"
				: "=a"(low), "=d"(high)
				: "c"(msr));
				return ((uint64_t)high << 32) | low;
			}

			static inline void WriteMsr(uint64_t msr, uint64_t value)
			{
				uint64_t low = value & 0xFFFFFFFFULL;
				uint64_t high = value >> 32;
				asm volatile (
					"wrmsr"
					:
					: "c"(msr), "a"((uint32_t)low), "d"((uint32_t)high)
				);
			}

			static inline uint64_t SystemCall() { uint64_t ret; asm volatile ( " syscall " : "=a"(ret) : : "rcx", "r11", "memory"); return ret; }
    };
}

#endif
