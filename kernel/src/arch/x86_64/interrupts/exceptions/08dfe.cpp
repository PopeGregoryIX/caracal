#include <stdint.h>
#include <stddef.h>
#include <exceptions.h>
#include <registers.h>
#include <memorylayout.h>

namespace arch
{
    Registers* Exceptions::DoubleFaultExceptionHandler(Registers* registers)
    {
        FATAL("Unhandled Double Fault Exception");
        for(;;);
        return registers;
    }
}