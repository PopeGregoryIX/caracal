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
        for(int i = 0; i < bitmapPages; i++) SlowPaging::PageIn2m(PAGE_PRESENT | PAGE_GLOBAL | PAGE_LARGE, MEMRANGE_BITMAP + (i * 0x200000), MemoryArray::AllocateMemoryLarge());
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
        for(int i = 0; i < PML4_INDEX(MEMRANGE_LFB); i++) pml4[i] = 0;    //  page out all of user space

        //  Map all of physical RAM to 0xFFFF FA00 0000 0000
        uintptr_t memorySize = MemoryArray::GetInstance().GetHighestAddress();
        uint64_t physicalMax = MEMRANGE_PHYSICAL + memorySize;
        if(physicalMax > MEMRANGE_LFB) FATAL("Not enough virtual space has been allocated to map physical RAM.");
        if(physicalMax % 0x200000) physicalMax+= ((0x200000) - physicalMax % 0x200000);

        //  CREATE NEEDED PDPT's //////////////////////////////////////////////////////////////
        uint64_t pdptNum = memorySize / PDPT_RANGE;
        if((pdptNum == 0) || memorySize % PDPT_RANGE) pdptNum++;
        uintptr_t tmpPdpt = (PageFrameAllocator::GetInstance().Allocate(0x1000 * pdptNum));
        if(tmpPdpt == 0) FATAL("Unable to allocate memory for PDPT's.");

        for(int i = 0; i < pdptNum; ++i) 
            pml4[i + PML4_INDEX(MEMRANGE_PHYSICAL)] = (tmpPdpt + (0x1000 * i)) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;

        X86_64_Utilities::WriteCr3(X86_64_Utilities::ReadCr3());

        //  CREATE NEEDED PD's //////////////////////////////////////////////////////////////
        uintptr_t pdpt = V_PDPT_4K(MEMRANGE_PHYSICAL);
        uint64_t pdNum = memorySize / PD_RANGE;
        if((pdNum == 0) || memorySize % PD_RANGE) pdNum++;
        uintptr_t tmpPd = (PageFrameAllocator::GetInstance().Allocate(0x1000 * pdNum));
        if(tmpPd == 0) FATAL("Unable to allocate memory for PD's.");
        for (int i = 0; i < pdNum; ++i) ((uint64_t*)pdpt)[i] = (tmpPd + (0x1000 * i)) | PAGE_PRESENT | PAGE_WRITE;
        if(pdNum % 0x200)
        {
            int topEntry = pdNum + (0x200 - (pdNum % 0x200));
            for(int i = pdNum; i < topEntry; ++i) ((uint64_t*)pdpt)[i] = 0;
        }


        X86_64_Utilities::WriteCr3(X86_64_Utilities::ReadCr3());

        //  CREATE NEEDED PDE's //////////////////////////////////////////////////////////////
        uintptr_t pd = V_PD_4K(MEMRANGE_PHYSICAL);
        uint64_t pdEntries = memorySize / PT_RANGE;
        if((pdEntries == 0) || memorySize % PT_RANGE) pdEntries++;
        for(int i = 0; i < pdEntries; i++) ((uint64_t*)pd)[i] = (0x200000 * i) | PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE;
        X86_64_Utilities::WriteCr3(X86_64_Utilities::ReadCr3());
        if(pdEntries % 0x200)
        {
            int topEntry = pdEntries + (0x200 - (pdEntries % 0x200));
            for(int i = pdEntries; i < topEntry; i++) ((uint64_t*)pd)[i] = 0;

        }
        
        //  Ensure PML4 entries all exist for supervisor space
        for(int i = PML4_INDEX(MEMRANGE_SUPERVISOR); i < 0x1FE; i++)
        {
            if((pml4[i] & PAGE_PRESENT) == 0)
            {
                uintptr_t physMem = PageFrameAllocator::GetInstance().AllocateEmpty();
                if(physMem == 0) FATAL("Unable to allocate physical RAM for a kernel PDPT.");
                pml4[i] = physMem | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
            }
        }
    }
}