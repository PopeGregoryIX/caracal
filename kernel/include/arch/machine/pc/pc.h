/*
 * pc.h
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_SRC_ARCH_MACHINE_PC_PC_H_
#define KERNEL_SRC_ARCH_MACHINE_PC_PC_H_

#include <machine.h>
#include <cpu.h>
#include <x86_64.h>

extern "C" void __acquireLock(uintptr_t* lock);
extern "C" void __releaseLock(uintptr_t* lock);

namespace arch
{
	class Pc : public Machine
	{
	public:
		inline Pc() : _cpuCount(0) {}

		static Pc& GetPcInstance( void ) { return _instance; }

		inline void AcquireLock(uintptr_t* lock) { __acquireLock(lock); }
		
		bool ApBoot( void );

		bool Boot( void );

		void AddDefaultConsoleDevices( DebugConsole& console );

		inline Cpu& GetCpu( void ) { return GetCpu(Cpu::CurrentProcessorId());}

		Cpu& GetCpu(uintptr_t id);

		inline size_t GetCpuCount( void ) { return _cpuCount; }

		void HaltCurrentCore()	{	asm("cli\nhlt");	}

		inline void ReleaseLock(uintptr_t* lock) { __releaseLock(lock); }
	private:
		void AddCpu(X86_64 cpu);

		static Pc _instance;
		size_t _cpuCount;
		X86_64* _cpus;
	};
}


#endif /* KERNEL_SRC_ARCH_MACHINE_PC_PC_H_ */
