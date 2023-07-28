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
#include <cpu.h>
#include <cboot.h>
#include <cpuutilities.h>
#include <debug.h>
#include <debug/lfbconsoleoutput.h>
#include <spinlock.h>

DebugConsole& debug = DebugConsole::GetInstance();
LfbConsoleOutput lfb;
bool bspDone = false;
Spinlock bootLock;

/**
 * @brief Kernel entry point.
 * 
 * Multi-platform entry point as loaded by the CBoot loader. This is 
 * a well-defined protocol for each platform. Platform-dependent code in
 * Machine and arch is called to perform the actual boot. Note
 * that on an SMP system, all cores may call kmain at the same time.
 * To port to another platform:
 * 	- Implement the Machine object.
 */
void kmain(CBoot* cboot)
{
	CpuUtilities::SetBspId(cboot->bspId);

	if(CpuUtilities::IsBsp())
	{
		_init();
		bootLock.Acquire();

		lfb.Initialise(	cboot->lfbAddress, 
						(const psf2_t*)&_binary_src_data_font_psf_start, 
						cboot->lfbScreenWidth, 
						cboot->lfbScreenHeight, 
						cboot->lfbScanlineBytes, 4);
		debug.AddOutputDevice(lfb);

		arch::Cpu::EarlyMemorySetup(*cboot);

		INFO("Caracal Kernel v0.2.0");
	}
	else
	{
		while(!bspDone){}
		bootLock.Acquire();
	}

	bspDone = true;
	bootLock.Release();

	FATAL("End of kernel main routine reached.");

	_fini();
}