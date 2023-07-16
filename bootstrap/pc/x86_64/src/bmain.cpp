#include <caracal.h>
#include <cpuid.h>
#include <bootboot.h>
#include <elf64.h>
#include <tar.h>
#include <runtime/cxx.h>
#include <support/string.h>
#include <debug/debug.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <memory/pageframeallocator.h>
#include <memory/memorylayout.h>

DebugConsole& debug = DebugConsole::GetInstance();
int tar_lookup(unsigned char *archive, char *filename, char **out);

void PageInLarge(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress);
uint64_t ReadCr3(void);
void WriteCr3(uint64_t value);
pageDirectoryEntry_t* GetNewPagingStructure( void );

void bmain( void )
{
    volatile uint32_t discard, ebx;
	__cpuid(1, discard, ebx, discard, discard);
    int id = ((ebx >> 24) & 0xFF);

    if(id == bootboot.bspid)
    {
        _init();

        debug.AddOutputDevice(LfbConsoleOutput::GetInstance());
        INFO("Caracal bootstrap.");

        PageFrameAllocator& pageFrameAllocator = ::PageFrameAllocator::GetInstance();
		pageFrameAllocator.Initialise(0x1000);
        
        Tar initRd((void*)bootboot.initrd_ptr);
        uintptr_t kernelPointer = initRd.GetEntryHandle("sys/kernel");
        if(kernelPointer == 0) FATAL("Kernel not found on Initial RamDisk.");
        INFO("Kernel size is: " << initRd.GetFileSize(kernelPointer));

        //  Load the kernel ELF file
        Elf64 kernel((void*)initRd.GetFileData(kernelPointer));
        if(!kernel.IsValid(ELF_BITS_64, ELF_LITTLE_ENDIAN)) FATAL("Kernel is not a valid 64 bit, little-endian ELF image.");
        if(kernel.GetArchitecture() != ELF_ARCH_X86_64) FATAL("Kernel image is not a valid x86-64 image.");

        //  Read ELF data and page in the kernel
        uintptr_t base = kernel.GetLowestAddress();
        uintptr_t top = base + kernel.GetMemorySize();
        uintptr_t requiredPages = (top - base) / 0x200000;
        if(((top - base) % 0x200000) != 0) requiredPages++;

        uintptr_t physMem = PageFrameAllocator::GetInstance().Allocate(requiredPages * 0x200000, 0x200000);
        for(uintptr_t i = 0; i < requiredPages; i++)
        {
            PageInLarge(PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE | PAGE_LARGE, 
            base + (i * 0x200000), physMem + (i * 0x200000));
        }
        
        INFO("Symbol gdt is at " << kernel.GetSymbolAddress("gdtSpace"));

        //  Relocate the kernel
        kernel.Relocate();

        //  Create space for a GDT per core

        //  Launch the kernel
        kernel.Start();
    }

    for(;;);    
}

void PageInLarge(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress)
{
    uint64_t* pml4 = (uint64_t*)ReadCr3();
    
    if(virtualAddress == UINT64_MAX) FATAL("Unable to page in - no physical memory allocated.");
    if((virtualAddress % 0x200000ULL) != 0) FATAL("Please align virtual address before paging in! At:" << virtualAddress);
    if((physicalAddress % 0x200000ULL) != 0) FATAL("Please align physical address before paging in! At:" << physicalAddress);

    uint64_t* pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
    if(pdpt == nullptr)
    {
        pml4[PML4_INDEX(virtualAddress)] = ((uint64_t)GetNewPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
        pdpt = (uint64_t*)(pml4[PML4_INDEX(virtualAddress)] & ~0xFFFULL);
    }

    uint64_t* pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
    if(pd == nullptr)
    {
        pdpt[PDPT_INDEX(virtualAddress)] = ((uint64_t)GetNewPagingStructure()) | PAGE_PRESENT | PAGE_GLOBAL | PAGE_WRITE;
        pd = (uint64_t*)(pdpt[PDPT_INDEX(virtualAddress)] & ~0xFFFULL);
    }

    uint64_t pde = (pd[PD_INDEX(virtualAddress)] & ~0xFFFULL);
    if(pde == 0)
        pd[PD_INDEX(virtualAddress)] = physicalAddress | flags;

    WriteCr3(ReadCr3());
}

uint64_t ReadCr3(void) { uint64_t value; asm volatile ( "movq %%cr3, %0" : "=r"(value) ); return value; }

void WriteCr3(uint64_t value) { asm volatile ( "movq %0, %%cr3" : : "a"(value) ); }

pageDirectoryEntry_t* GetNewPagingStructure( void )
{
    uintptr_t pageStructure = PageFrameAllocator::GetInstance().Allocate();
    if(pageStructure == UINT64_MAX) FATAL("Unable to find memory for a new paging structure.");

    ::memset((void*)pageStructure, 0, 0x1000);

    return (pageDirectoryEntry_t*)pageStructure;
}
