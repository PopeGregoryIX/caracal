#include <bmain.h>
#include <cboot.h>
#include <bootboot.h>
#include <cxx.h>
#include <cpuUtilities.h>
#include <spinlock.h>
#include <debug.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <memory/memoryarray.h>
#include <tar.h>
#include <elf64.h>

bool bspInitialised = false;
Spinlock mainLock;
DebugConsole& debug = DebugConsole::GetInstance();
LfbConsoleOutput lfb;

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
    MemoryArray& mmap = MemoryArray::GetInstance();
    Tar initRd((void*)bootboot.initrd_ptr);

    if(CpuUtilities::IsBsp())
    {
        _init();
        mainLock.Acquire();

        //  Ensure we can display debug info               
        lfb.Initialise(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
        debug.AddOutputDevice(lfb);

        //  Allocate (slowly!) using the system memory map for now
        mmap.Initialise(&(bootboot.mmap));

        //  Locate the kernel on the initrd
        INFO(initRd.GetCurrentDirectorySize());
        initRd.PrintDirectoryListing();
        uintptr_t kernelPointer = initRd.GetEntryHandle("sys/kernel");

        if(kernelPointer == 0)
            FATAL("Unable to locate sys/kernel on initial RAM disk.");
        
        INFO("BSP (" << (uintptr_t)processorId << ") OK");    
        bspInitialised = true;
    }
    else
    {
        while(!bspInitialised){}
        mainLock.Acquire();
        INFO("AP (" << (uintptr_t)processorId << ") OK");    
    }

    mainLock.Release();
    
    for(;;) {}

}