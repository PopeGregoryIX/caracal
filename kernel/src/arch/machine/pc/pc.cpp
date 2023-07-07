/*
 * pc.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */
#include <machine.h>
#include <caracal.h>
#include <pc.h>
#include <runtime/cxx.h>
#include <x86_64.h>
#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include <debug/lfbconsoleoutput.h>
#include <tables/gdt.h>
#include <tables/idt.h>
#include <memory/pageframeallocator.h>
#include <process/processmanager.h>
#include <process/userfunctions.h>
#include <memory/memoryarray.h>
#include <memory/memorylayout.h>
#include <memory/heapmanager.h>
#include <interrupt/exceptions.h>
#include <interrupt/interrupts.h>
#include <memory/virtualmemorymanager.h>

Machine& Machine::GetInstance( void ) { return arch::Pc::GetPcInstance(); }

extern "C" uint8_t mmio;
extern "C" uint8_t kernelStart;
extern "C" uint8_t kernelEnd;
extern "C" void _setBspDone( void );

namespace arch
{
	Pc Pc::_instance;

	void Pc::AddDefaultConsoleDevices(DebugConsole& console)
	{
		console.AddOutputDevice(LfbConsoleOutput::GetInstance());
	}

	bool Pc::Boot( void )
	{
		//	This is where we set up the machine. This is purely called
		//	by the BSP.
		INFO("Running boot routines for machine type: PC");
		
		//	1. Initialise the memory map - we need to be able to allocate physical pages
		VINFO("Initialise Page Frame Allocator");
		PageFrameAllocator& pageFrameAllocator = ::PageFrameAllocator::GetInstance();
		pageFrameAllocator.Initialise(0x1000);

		//	2. Initialise CPU structures, such as GDT / IDT / TSS etc...
		INFO("Initialise BSP");
		X86_64 bspCpu;
		AddCpu(bspCpu);

		//	Signal AP's to boot
		INFO("Signal AP's");
		_setBspDone();
		return true;
	}

	bool Pc::ApBoot( void )
	{
		_bootLock.Acquire();
		X86_64 cpu;

		//	create a copy of PML4
		INFO("Setting up PML4");
		pageDirectoryEntry_t* pml4 = HeapManager::GetNewPagingStructure();
		memorycopy<pageDirectoryEntry_t>(pml4, (pageDirectoryEntry_t*)X86_64::ReadCr3(), 0x200);
		X86_64::WriteCr3((uintptr_t)pml4);

		_bootLock.Release();
		return true;
	}


	Cpu& Pc::GetCpu(uintptr_t id)
	{
		INFO("CPUS is " << (uintptr_t)_cpus);
		for (size_t i = 0; i < _cpuCount; ++i)
		{
			INFO("Requested ID: " << (uintptr_t)id << ", actual ID: " << (uintptr_t)_cpus[i].GetId());
			if(_cpus[i].GetId() == id) return _cpus[i];
		}
		
		FATAL("Cpu not found with id " << id);
		return (Cpu&)*((Cpu*)nullptr);
	}

	void Pc::AddCpu(X86_64 cpu)
	{
		X86_64* newCpus = (X86_64*)VirtualMemoryManager::GetInstance().GetKernelAllocator().Allocate(sizeof(X86_64) * (_cpuCount + 1));
		if(newCpus == nullptr) FATAL("Unable to allocate memory for CPU structures.");

		if(_cpus != nullptr)
		{
			 memorycopy<X86_64>(newCpus, _cpus, _cpuCount);
			 delete _cpus;
		}

		memorycopy<X86_64>(&(newCpus[_cpuCount++]), &cpu, 1);

		_cpus = newCpus;
	}
}
