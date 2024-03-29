#include <bmain.h>
#include <cboot.h>
#include <caracal.h>
#include <bootboot.h>
#include <cxx.h>
#include <arch/cpuutilities.h>
#include <memory/spinlock.h>
#include <debug.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <memory/memoryarray.h>
#include <format/tar.h>
#include <format/elf64.h>
#include <memorylayout.h>

namespace arch
{
    void AllocatorSetup( void );
    void SetupArchitectureStructures( void );
    void LaunchKernel( void (*kernelEntry)() );
}

bool bspInitialised = false;
Spinlock mainLock;
DebugConsole& debug = DebugConsole::GetInstance();
LfbConsoleOutput lfb;
void (*kernelEntry)();

/**
 * @brief Bootstrap entry point.
 * 
 * It is ultimately hoped that the bootstrap will be able to handle various loaders.
 * For now, bootboot is the only supported loader.
*/
void bmain( void )
{
    CpuUtilities::SetBspId(bootboot.bspid);
    int processorId = CpuUtilities::GetCurrentProcessorId();
    Tar initRd((void*)bootboot.initrd_ptr);

    if(CpuUtilities::IsBsp())
    {
        _init();
        mainLock.Acquire();

        //  Ensure we can display debug info               
        lfb.Initialise(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
        debug.AddOutputDevice(lfb);

        //  Allocate (slowly!) using the system memory map for now
        arch::AllocatorSetup();
        
        //  Locate the kernel on the initrd
        uintptr_t kernelPointer = initRd.GetEntryHandle("sys/kernel");
        if(kernelPointer == 0) FATAL("Unable to locate sys/kernel on initial RAM disk.");
        Elf64 kernel((void*)initRd.GetFileData(kernelPointer));
        if(!kernel.IsValid()) FATAL("Kernel is not a valid ELF64 file.");

        //  Arch-Dependent - ensure any virtual memory mapping is done
        CpuUtilities::EnsureMemoryAccessible(kernel.GetLowestAddress(), kernel.GetMemorySize(), CARACAL_MEMORY_KERNEL_CODE);

        //  Relocate the kernel and provide an entry point for all cores
        kernel.Relocate();
        kernelEntry = (void (*)())kernel.GetEntry();

        //  Allow AP's to continue
        bspInitialised = true;
    }
    else
    {
        while(!bspInitialised){}
        mainLock.Acquire();   
    }

    //  Allow Architecture-Dependent Objects to be Created
    
    arch::SetupArchitectureStructures();
    mainLock.Release();
    arch::LaunchKernel(kernelEntry);
}