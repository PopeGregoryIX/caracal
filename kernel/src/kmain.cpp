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
void kmain()
{
	for(;;) {}
}