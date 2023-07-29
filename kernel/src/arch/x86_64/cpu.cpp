#include <arch/cpu.h>
#include <x86_64.h>
#include <cboot.h>
#include <debug.h>
#include <idt.h>
#include <exceptions.h>

namespace arch
{
    void Cpu::EarlyMemorySetup(CBoot& cboot)
    {
        Idt& idt = Idt::GetInstance();
        idt.Load();
        idt.InstallExceptionHandler(0x08, Exceptions::DoubleFaultExceptionHandler);
        idt.InstallExceptionHandler(0x0E, Exceptions::PageFaultExceptionHandler);
        
    }
}