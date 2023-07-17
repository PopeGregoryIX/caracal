#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <interrupt/exceptions.h>
#include <registers.h>
#include <memorylayout.h>
#include <memory/virtualmemorymanager.h>
#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>

namespace arch
{
    Registers* Exceptions::DoubleFaultExceptionHandler(Registers* registers)
    {
        FATAL("Unhandled Double Fault Exception");
        for(;;);
        return registers;
    }
}
