#include <bmain.h>
#include <cboot.h>
#include <bootboot.h>
#include <cxx.h>
#include <cpuUtilities.h>
#include <spinlock.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>

bool _bspInitialised = false;
Spinlock _mainlock;

void bmain( void )
{
    DebugConsole _debugConsole;
    CpuUtilities::SetBspId(bootboot.bspid);
    int processorId = CpuUtilities::GetCurrentProcessorId();

    if(CpuUtilities::IsBsp())
    {
        _mainlock.Acquire();
        _init();
        LfbConsoleOutput _lfb(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
        _debugConsole.AddOutputDevice(_lfb);
        _debugConsole << (uint64_t)processorId << '\n';    
        _bspInitialised = true;
    }
    else
    {
        while(!_bspInitialised){}
        _mainlock.Acquire();

        LfbConsoleOutput _lfb(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
        _debugConsole.AddOutputDevice(_lfb);
        for(int i = 0; i < processorId; i++) _debugConsole << '\n';
        _debugConsole << (uint64_t)processorId << '\n';  
    }

    _mainlock.Release();
    
    for(;;) {}

}