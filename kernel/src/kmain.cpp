/**
 * @file kmain.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains the multi-platform kernel entry point (use the BOOTBOOT bootloader).
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <caracal.h>
#include <kernel.h>
#include <cboot.h>
#include <arch/glue.h>
#include <arch/cpuutilities.h>
#include <memory/spinlock.h>
#include <debug.h>
#include <debug/lfbconsoleoutput.h>
#include <arch/cpu.h>

bool bspInitialised = false;
Spinlock mainLock;
DebugConsole& debug = DebugConsole::GetInstance();
LfbConsoleOutput lfb;

/**
 * @brief Kernel entry point.
 * 
 * Multi-platform entry point as loaded by the BOOTBOOT loader. This is 
 * a well-defined protocol for each platform. Platform-dependent code in
 * Machine and arch is called to perform the actual boot. Note
 * that on an SMP system, all cores may call kmain at the same time.
 * To port to another platform:
 * 	- Implement the Machine object.
 * 	- Implement archdef.h.
 */
void kmain(CBoot* cbootPtr)
{
	CBoot& cboot = *cbootPtr;
	
	if(CpuUtilities::IsBsp())
	{
		_init();

		mainLock.Acquire();
		lfb.Initialise(	cboot.lfbAddress, (const psf2_t*)&_binary_src_data_font_psf_start, 
						cboot.lfbScreenWidth, cboot.lfbScreenHeight, cboot.lfbScanlineBytes, 4);
		debug.AddOutputDevice(lfb);

		INFO("Caracal Kernel Version 0.2.0");
		arch::Glue::EarlyMemorySetup(cboot);
		bspInitialised = true;
	}
	else
	{
		while(!bspInitialised) {}
		arch::Glue::APSetup(cboot);
	}

	FATAL("End of kmain reached");
	for(;;) {}
}