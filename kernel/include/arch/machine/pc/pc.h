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

namespace arch
{
	class Pc : public Machine
	{
	public:
		inline Pc() : cpuCount_(0) {}

		inline Cpu& GetCpu( void ) { return GetCpu(Cpu::CurrentProcessorId());}

		Cpu& GetCpu(uintptr_t id);

		inline size_t GetCpuCount( void ) { return cpuCount_; }

		static Pc& GetPcInstance( void ) { return _instance; }

		bool Boot( void );

		void AddDefaultConsoleDevices( DebugConsole& console );

		void CreateKernelMemorySpace( void );

		void HaltCurrentCore()	{	asm("cli\nhlt");	}
	private:
		void AddCpu(X86_64 cpu);

		static Pc _instance;
		size_t cpuCount_;
		X86_64* cpus_;
	};
}


#endif /* KERNEL_SRC_ARCH_MACHINE_PC_PC_H_ */
