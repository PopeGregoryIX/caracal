#include <caracal.h>
#include <cpuid.h>
#include <bootboot.h>
#include <tar.h>
#include <runtime/cxx.h>
#include <support/string.h>
#include <debug/debug.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <memory/pageframeallocator.h>

DebugConsole& debug = DebugConsole::GetInstance();
int tar_lookup(unsigned char *archive, char *filename, char **out);

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
        
    }

    for(;;);    
}