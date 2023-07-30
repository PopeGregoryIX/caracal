#include <stdint.h>
#include <stddef.h>
#include <arch/cpuutilities.h>
#include <memorylayout.h>
#include <gdt.h>
#include <cboot.h>
#include <caracal.h>
#include <debug.h>
#include <bootboot.h>
#include <paging.h>
#include <memory/memoryarray.h>
#include <x86_64_utilities.h>
#include <memory/spinlock.h>

namespace arch
{
    bool mmapCopied = false;
    Spinlock mmapLock;

    void LaunchKernel( void (*kernelEntry)() )
    {
        mmapLock.Acquire();
        if(!mmapCopied)
        {
            //  By copying the memoryMap now, we don't risk allocations happening before launching the kernel.
            CBoot* cboot = (CBoot*)MEMRANGE_CBOOT;
            cboot->mmapAddress = MEMRANGE_MMAP;
            cboot->mmapBytes = MemoryArray::GetInstance().Size();
            memcpy((void*)MEMRANGE_MMAP, MemoryArray::GetInstance().GetFirst(), cboot->mmapBytes);
            mmapCopied = true;
        }

        mmapLock.Release();

        __stackReset(MEMRANGE_STACK_TOP(CpuUtilities::GetCurrentProcessorId()), (uintptr_t)kernelEntry, MEMRANGE_CBOOT);
    }
}