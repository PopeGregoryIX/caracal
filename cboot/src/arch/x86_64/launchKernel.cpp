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
#include <x86_64.h>

namespace arch
{
    void LaunchKernel( void (*kernelEntry)() )
    {
        //  Page in and copy the Memory Map
        uintptr_t mmap_pages = MemoryArray::GetInstance().Size() / 0x200000;
        if((MemoryArray::GetInstance().Size() % 0x200000) != 0) mmap_pages++;

        for(int i = 0; i < mmap_pages; i++)
        {
            Paging::PageIn4k(PAGE_PRESENT | PAGE_WRITE, MEMRANGE_MMAP + (i * 0x1000), MemoryArray::AllocateMemorySmall());

            //  the act of paging in can change the size of the mmap!
            mmap_pages = MemoryArray::GetInstance().Size() / 0x200000;
            if((MemoryArray::GetInstance().Size() % 0x200000) != 0) mmap_pages++;
        }

        memcpy((void*)MEMRANGE_MMAP, (void*)MemoryArray::GetInstance().GetFirst(), MemoryArray::GetInstance().Size());

        CBoot* cboot = (CBoot*)MEMRANGE_CBOOT;
        cboot->mmapAddress = (MemoryMapEntry*)MEMRANGE_MMAP;
        cboot->mmapBytes = MemoryArray::GetInstance().Size();

        __stackReset(MEMRANGE_STACK_TOP(CpuUtilities::GetCurrentProcessorId()), (uintptr_t)kernelEntry, MEMRANGE_CBOOT);
    }
}