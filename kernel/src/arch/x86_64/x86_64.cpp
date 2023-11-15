#include <x86_64.h>
#include <arch/cpu.h>
#include <x86_64.h>
#include <x86_64_utilities.h>
#include <cboot.h>
#include <debug.h>
#include <idt.h>
#include <exceptions.h>
#include <memory/memoryarray.h>
#include <memory/pageframeallocator.h>
#include <memorylayout.h>
#include <slowPaging.h>
#include <arch/cpuutilities.h>
#include <memory/memoryallocator.h>
#include <arch/machine.h>
#include <memory/heapmanager.h>

namespace arch
{
    Spinlock __cpu__Early_Lock;

    void X86_64::APSetup(CBoot& cboot)
    {
        __cpu__Early_Lock.Acquire();

        X86_64_Utilities::WriteCr3(X86_64_Utilities::ReadCr3());
        Idt& idt = Idt::GetInstance();
        idt.Load();

        __cpu__Early_Lock.Release();
    }

    void X86_64::IdtSetup( void )
    {
        Idt& idt = Idt::GetInstance();
        idt.Load();
        idt.InstallExceptionHandler(0x08, Exceptions::DoubleFaultExceptionHandler);
        idt.InstallExceptionHandler(0x0E, Exceptions::PageFaultExceptionHandler);
    }

    void X86_64::PageFrameAllocationSetup(CBoot& cboot)
    {
        MemoryArray& memoryArray = MemoryArray::GetInstance();
        memoryArray.Initialise((MemoryMapEntry*)cboot.mmapAddress, cboot.mmapBytes, cboot.mmapLimit);

        //  Set up a space for the memory bitmap
        //  Prepare a page frame memory bitmap
        SlowPaging::SetGetPageFunction(MemoryArray::AllocateMemorySmall);
        CpuUtilities::SetPageRoutines(MemoryArray::AllocateMemorySmall, MemoryArray::AllocateMemoryLarge);

        size_t bitmapSizeBytes = ((memoryArray.GetHighestAddress() / 0x1000) / sizeof(uint64_t));
        if(bitmapSizeBytes % sizeof(uint64_t)) bitmapSizeBytes += sizeof(uint64_t) - (bitmapSizeBytes % sizeof(uint64_t));
        size_t bitmapPages = bitmapSizeBytes / 0x200000;
        if((bitmapSizeBytes % 0x200000) != 0) bitmapPages++;
        for(int i = 0; i < bitmapPages; i++) SlowPaging::PageIn2m(PAGE_PRESENT | PAGE_GLOBAL, MEMRANGE_BITMAP + (i * 0x200000), MemoryArray::AllocateMemoryLarge());
        PageFrameAllocator::GetInstance().Initialise(0x1000, memoryArray, MEMRANGE_BITMAP);
    }

    void X86_64::PagingSetup(CBoot& cboot)
    {
        //  Set up our new memory space. 
        //  Paging structures will now be recursive and anything in user space should be cleared.
        uint64_t* pml4 = (uint64_t*)X86_64_Utilities::ReadCr3();
        pml4[PML4_INDEX(0xFFFFFF8000000000)] = ((uintptr_t)pml4) | PAGE_PRESENT | PAGE_WRITE;
        X86_64_Utilities::WriteCr3((uintptr_t)pml4);
        pml4 = (uint64_t*)V_PML4_4K;
        for(int i = 0; i < PML4_INDEX(MEMRANGE_SUPERVISOR); i++) pml4[i] = 0;    //  page out all of user space

        for(int i = PML4_INDEX(MEMRANGE_SUPERVISOR); i < 0x1FF ; i++)
        {
            //  Map all of physical RAM to 0xFFFF FA00 0000 0000
            uint64_t physicalMax = MEMRANGE_PHYSICAL + MemoryArray::GetInstance().GetHighestAddress();
            if(physicalMax > MEMRANGE_LFB) FATAL("Not enough virtual space has been allocated to map physical RAM.");
            

            
            //  Ensure PML4 entries all exist for supervisor space   
        }
    }
}