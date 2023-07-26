#include <bmain.h>
#include <cboot.h>
#include <bootboot.h>
#include <cxx.h>
#include <cpuUtilities.h>
#include <spinlock.h>
#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>

DebugConsole _console;
bool _bspInitialised = false;
Spinlock _mainlock;

void bmain( void )
{
    CpuUtilities::SetBspId(bootboot.bspid);
    LfbConsoleOutput _lfb(bootboot.fb_ptr, (const psf2_t*)&_binary_src_data_font_psf_start, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline, 4);
    _console.AddOutputDevice(_lfb);

    if(CpuUtilities::IsBsp())
    {
        _init();
        _mainlock.Acquire();
        _console.PutDecimal(CpuUtilities::GetCurrentProcessorId());   
        _bspInitialised = true;
        _mainlock.Release();
    }
    else
    {
        while(!_bspInitialised){}
        _mainlock.Acquire();
        _console.PutDecimal(CpuUtilities::GetCurrentProcessorId());
        _mainlock.Release();
    }
    


    for(;;) {}
}