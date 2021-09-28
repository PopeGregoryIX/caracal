#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <interrupt/exceptions.h>
#include <registers.h>

namespace arch
{
    Registers* Exceptions::PageFaultExceptionHandler(Registers* registers)
    {
        uintptr_t faultAddress = CPU_CLASS::ReadCr2();
        INFO("Page Fault Exception at: " << faultAddress << " with error code " << registers->errorCode);
        
        INFO(" ");
        WARNING("PFE unhandled. Dumping core:\n\n");
        Exceptions::DumpCore(registers);
        
        FATAL("Unhandled PFE");
        return registers;
    }
}
