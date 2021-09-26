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
#include <archdef.h>
#include <stdint.h>
#include <stddef.h>
#include <interrupt/exceptions.h>
#include <registers.h>
#include <debug/debug.h>
namespace arch
{
    const char* Exceptions::_ExceptionNames[32] = {	"Divide Error", "Debug", "Non-maskable Interrupt", "Breakpoint",
												"Overflow", "Boundary Range Exceeded", "Undefined Opcode", "Device Unavailable",
												"Double Fault", "Reserved", "Invalid TSS", "Not Present", "Stack Segment",
												"General Protection", "Page Fault", "Reserved", "Math Fault", "Alignment Check",
												"Machine Check", "Extended Math Fault", "Virtualisation", "Reserved",
												"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
												"Reserved", "Reserved", "Reserved", "Reserved"
											};

    Registers* Exceptions::DefaultHandler(Registers* regs)
    {
        INFO("");
        INFO(ConsoleColour(0xFF0000, 0x000000) << _ExceptionNames[regs->interruptNumber] << " Exception");

        INFO("Registers stored at: " << (uint64_t)regs);
        
        PRINTREGS("RAX", regs->rax, "RBX", regs->rbx);
        PRINTREGS("RCX", regs->rcx, "RDX", regs->rdx);
        PRINTREGS("RSI", regs->rsi, "RDI", regs->rdi);
        PRINTREGS("R8", regs->r8, "R9", regs->r9);
        PRINTREGS("R10", regs->r10, "R11", regs->r11);
        PRINTREGS("R12", regs->r12, "R13", regs->r13);
        PRINTREGS("R14", regs->r14, "R15", regs->r15);

        INFO("");

        PRINTREGS("UserRSP", regs->userRsp, "RBP", regs->rbp);

        PRINTREGS("CS", regs->cs, "EIP", regs->rip);
        PRINTREGS("Code", regs->errorCode, "Number", regs->interruptNumber);
        

        FATAL("Unhandled CPU Exception.");
        return regs;
    }
}