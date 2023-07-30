/**
 * @file exceptions.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief 
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include <exceptions.h>
#include <registers.h>
#include <debug.h>
#include <arch/cpu.h>
#include <x86_64_utilities.h>
#include <memory/spinlock.h>
#include <arch/cpuutilities.h>

namespace arch
{
    Spinlock Exceptions::_coreDumpLock;
    
    const char* Exceptions::_ExceptionNames[32] = {	"Divide Error", "Debug", "Non-maskable Interrupt", "Breakpoint",
												"Overflow", "Boundary Range Exceeded", "Undefined Opcode", "Device Unavailable",
												"Double Fault", "Reserved", "Invalid TSS", "Not Present", "Stack Segment",
												"General Protection", "Page Fault", "Reserved", "Math Fault", "Alignment Check",
												"Machine Check", "Extended Math Fault", "Virtualisation", "Reserved",
												"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
												"Reserved", "Reserved", "Reserved", "Reserved"
											};

    Registers* Exceptions::DefaultHandler(Registers* registers)
    {
        DebugConsole& debug = DebugConsole::GetInstance();
        
        debug << "\n" << ConsoleColour(0xFF0000, 0x000000) << "Unhandled " << _ExceptionNames[registers->interruptNumber] << " Exception (core " << (uintptr_t)CpuUtilities::GetCurrentProcessorId() << ")\n\n";
        DumpCore(registers);        

        FATAL("Unhandled Exception");
        return registers;
    }

    void Exceptions::DumpCore(Registers* registers)
    {
        _coreDumpLock.Acquire();

        DebugConsole& debug = DebugConsole::GetInstance();
        
        PrintRegisters("RAX", registers->rax, "RBX", registers->rbx);
        PrintRegisters("RCX", registers->rcx, "RDX", registers->rdx);
        PrintRegisters("RSI", registers->rsi, "RDI", registers->rdi);
        PrintRegisters("R8", registers->r8, "R9", registers->r9);
        PrintRegisters("R10", registers->r10, "R11", registers->r11);
        PrintRegisters("R12", registers->r12, "R13", registers->r13);
        PrintRegisters("R14", registers->r14, "R15", registers->r15);

        debug << "\n";

        PrintRegisters("RSP", registers->userRsp, "SS", registers->userSs);
        PrintRegisters("RBP", registers->rbp, "DS", 0);

        debug << "\n";

        PrintRegisters("CS", registers->cs, "EIP", registers->rip);
        PrintRegisters("Code", registers->errorCode, "Num", registers->interruptNumber);

        debug << "\n";
        
        PrintRegisters("CR0", X86_64_Utilities::ReadCr0(), "CR2", X86_64_Utilities::ReadCr2());
        PrintRegisters("CR3", X86_64_Utilities::ReadCr3(), "CR4", X86_64_Utilities::ReadCr4());

        debug << "\nError Code: " << (uint64_t)registers->errorCode;

        debug << "\nStack stored at: " << (uint64_t)registers << "\n";
        
        _coreDumpLock.Release();
        /*debug << "First 10 stack frames:\n";

        
        uint64_t* stack = registers->Stack();
        for(size_t i = 0; i < 10; ++i)
        {
            debug << (uint64_t)stack << ":" << *stack << '\n';
            stack++;
        }*/
    }
}