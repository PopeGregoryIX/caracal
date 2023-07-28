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
    void SetupArchitectureStructures( void )
    {
        X86_64::WriteCr3(X86_64::ReadCr3());

        Gdt* gdt = (Gdt*)MEMRANGE_GDT;

        if(CpuUtilities::IsBsp())
        {
            CpuUtilities::EnsureMemoryAccessible(MEMRANGE_GDT, sizeof(Gdt) * CpuUtilities::GetProcessorCount(), CARACAL_MEMORY_KERNEL_DATA);

            //  Do per-CPU initialisation
            for(int i = 0; i < CpuUtilities::GetProcessorCount(); i++)
            {
                gdt[i].Initialise();

                uintptr_t cpuStack = MEMRANGE_STACK_BASE(i);
                Paging::PageIn2m(PAGE_PRESENT | PAGE_WRITE, cpuStack, MemoryArray::AllocateMemoryLarge());
            }

            // Page in the LFB
            uintptr_t lfb_physical = bootboot.fb_ptr;
            uintptr_t lfb_pages = bootboot.fb_size / 0x200000;
            if((bootboot.fb_size % 0x200000) != 0) lfb_pages++;
            for(int i = 0; i < lfb_pages; i++)
                Paging::PageIn2m(PAGE_PRESENT | PAGE_WRITE, MEMRANGE_LFB + (i * 0x200000), lfb_physical + (i * 0x200000));

            //  Page in the InitRd
            uintptr_t ird_mem = bootboot.initrd_ptr;
            uintptr_t ird_pages = bootboot.initrd_size / 0x200000;
            if((bootboot.initrd_size % 0x200000) != 0) ird_pages++;
            for(int i = 0; i < ird_pages; i++)
                Paging::PageIn2m(PAGE_PRESENT | PAGE_WRITE, MEMRANGE_INITRD + (i * 0x200000), MemoryArray::AllocateMemoryLarge());
            memcpy((void*)MEMRANGE_INITRD, (void*)ird_mem, bootboot.initrd_size);

            //  Page in the CBoot structure
            Paging::PageIn4k(PAGE_PRESENT | PAGE_WRITE, MEMRANGE_CBOOT, MemoryArray::AllocateMemorySmall());

            CBoot* cboot = (CBoot*)MEMRANGE_CBOOT;
            cboot->magic = 0x0CA8ACAl;
            cboot->version = 1;
            cboot->size = sizeof(CBoot);
            cboot->bspId;
            cboot->lfbAddress = MEMRANGE_LFB;
            cboot->lfbSize = bootboot.fb_size;
            cboot->lfbFormat = bootboot.fb_type;
            cboot->lfbScreenWidth = bootboot.fb_width;
            cboot->lfbScreenHeight = bootboot.fb_height;
            cboot->lfbScanlineBytes = bootboot.fb_scanline;
            cboot->mmapAddress = MEMRANGE_MMAP;
            cboot->mmapBytes = MemoryArray::GetInstance().Size();
            cboot->configStringAddress = MEMRANGE_CONFIG;
            cboot->configStringBytes = 0;
            cboot->initRdAddress = MEMRANGE_INITRD;
            cboot->initRdSize = bootboot.initrd_size;
            cboot->cbootArchData.cbootArchPC.gdtAddress = MEMRANGE_GDT;
            cboot->cbootArchData.cbootArchPC.gdtBytes = sizeof(Gdt) * CpuUtilities::GetProcessorCount();
        }
    
        gdt[CpuUtilities::GetCurrentProcessorId()].Load();
        gdt[CpuUtilities::GetCurrentProcessorId()].LoadTss();
    }
}
