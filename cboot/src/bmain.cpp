#include <bmain.h>
#include <cboot.h>
#include <bootboot.h>
#include <cxx.h>
#include <cpuUtilities.h>
#include <spinlock.h>
#include <debug.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>

bool _bspInitialised = false;
Spinlock _mainlock;
DebugConsole& debug = DebugConsole::GetInstance();

void bmain( void )
{
    CpuUtilities::SetBspId(bootboot.bspid);
    
    if(CpuUtilities::IsBsp())
    {
        _init();
        _mainlock.Acquire();
        
        LfbConsoleOutput _lfb(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
        debug.AddOutputDevice(_lfb);

        INFO("BSP OK");    
        _bspInitialised = true;
    }
    else
    {
        while(!_bspInitialised){}
        _mainlock.Acquire();
        //for(int i = 0; i < processorId; i++) _debugConsole << '\n';
        //_debugConsole << (uint64_t)processorId << '\n';  
    }

    _mainlock.Release();
    
    for(;;) {}

}