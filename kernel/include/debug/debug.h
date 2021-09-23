/*
 * debug.h
 *
 *  Created on: 23 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_DEBUG_H_
#define KERNEL_INCLUDE_DEBUG_DEBUG_H_


#include "debugconsole.h"
#include "consolecolour.h"
#include <machine.h>

#ifdef DEBUG
#define INFO(x)	{	DebugConsole::GetInstance() << ConsoleColour(0x00FF00, 0x000000) << "[INFO]    " << ConsoleColour(0xFFFFFF, 0x000000) << x << '\n';	}
#else
#define INFO(x)	{}
#endif

#ifdef DEBUG
#define WARNING(x)	{	DebugConsole::GetInstance() << ConsoleColour(0x0000FF, 0x000000) << DebugConsole::GetInstance() << "[WARN]    " << ConsoleColour(0xFFFFFF, 0x000000) << x << '\n';	}
#else
#define WARNING(x)	{}
#endif

#ifdef DEBUG
#define FATAL(x)	{	DebugConsole::GetInstance() << ConsoleColour(0xFF0000, 0x000000) << "\n[ERROR]   " << ConsoleColour(0xFFFFFF, 0x000000) << x; Machine::GetInstance().Halt();	}
#else
#define FATAL(x)	{}
#endif


#endif /* KERNEL_INCLUDE_DEBUG_DEBUG_H_ */
