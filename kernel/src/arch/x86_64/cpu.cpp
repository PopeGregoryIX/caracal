#include <arch/cpu.h>
#include <x86_64.h>
#include <cboot.h>
#include <debug.h>
#include <idt.h>
#include <exceptions.h>
#include <memory/memoryarray.h>

namespace arch
{
    void Cpu::EarlyMemorySetup(CBoot& cboot)
    {
        //  kmain already ensures that only the BSP calls this function.
        Idt& idt = Idt::GetInstance();
        idt.Load();
        idt.InstallExceptionHandler(0x08, Exceptions::DoubleFaultExceptionHandler);
        idt.InstallExceptionHandler(0x0E, Exceptions::PageFaultExceptionHandler);

        MemoryArray& memoryArray = MemoryArray::GetInstance();
        memoryArray.Initialise((MemoryMapEntry*)cboot.mmapAddress, cboot.mmapBytes, cboot.mmapLimit);
    }
}