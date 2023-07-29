#include <stdint.h>
#include <stddef.h>
#include <exceptions.h>
#include <registers.h>
#include <debug.h>

namespace arch
{
    Registers* Exceptions::PageFaultExceptionHandler(Registers* registers)
    {
        FATAL("Unhandled PFE");
        return registers;
    }
}