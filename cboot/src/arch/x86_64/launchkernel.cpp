#include <stdint.h>
#include <stddef.h>
#include <cpuutilities.h>
#include <memorylayout.h>
#include <gdt.h>
#include <cboot.h>
#include <caracal.h>
#include <debug.h>
#include <bootboot.h>
#include <paging.h>
#include <memory/memoryarray.h>
#include <x86_64_utilities.h>

namespace arch
{
    void LaunchKernel( void (*kernelEntry)() )
    {
        __stackReset(MEMRANGE_STACK_TOP(CpuUtilities::GetCurrentProcessorId()), (uintptr_t)kernelEntry, MEMRANGE_CBOOT);
    }
}