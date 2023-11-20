/**
 * \file glue.cpp
 * 
 * The header for the functions below are the same for each
 * architecture, but the glue.cpp file is different per-architecture.
 * 
 * This code links the arch-dependent and arch-independent portions of the kernel.
*/
#include <arch/glue.h>
#include <arch/cpu.h>
#include <arch/machine.h>
#include <x86_64.h>
#include <process/process.h>
#include <debug.h>
#include <x86_64_utilities.h>
#include <pc.h>
#include <service/syscall.h>

void Glue::AddCurrentCpu( void )
{
    arch::Pc::GetInstance().AddCurrentCpu();
}

void Glue::APSetup(CBoot& cboot)
{
    arch::X86_64::APSetup(cboot);
}

void Glue::EarlyMemorySetup(CBoot& cboot)
{
    //  kmain already ensures that only the BSP calls this function.
    arch::X86_64::IdtSetup();
    arch::X86_64::PageFrameAllocationSetup(cboot);
    arch::X86_64::PagingSetup(cboot);

    //  From now on, the bitmap page frame allocator and kernel pager (not "SlowPaging!") should be used.
    arch::Machine::GetHeapAllocator().Initialise();
}

void Glue::MachineSetup(CBoot& cboot)
{
    arch::Pc::GetInstance().Initialise(cboot);
}

Process& Glue::CreateProcess(bool supervisor, String name)
{
    Process* process = new Process( name );

    uintptr_t newPageMap = PageFrameAllocator::GetInstance().AllocateEmpty();
    if(newPageMap == 0) FATAL("Unable to create page map for new process.");
    uint64_t vPageMap = (uint64_t)GET_VIRTUAL_POINTER(newPageMap);
    memcpy((void*)(vPageMap + 0x800), (void*)(V_PML4_4K + 0x800), 0x800);
    process->_processInfo = newPageMap;
    
    INFO("Created process: " << process->GetName());

    return *process;
}

Process& Glue::GenerateInitialProcess( void )
{
    Process& init = CreateProcess(true, "Init");
    arch::Machine::GetInstance().GetCurrentCpu().SetCurrentThread(init._threads.GetFirst());
    return init;
}

void Glue::Yield( void )
{
    arch::X86_64_Utilities::SystemCall<SYSCALL_VECTOR>(SYSCALL_YIELD);
}