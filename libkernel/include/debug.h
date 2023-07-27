#ifndef __DEBUG___H__
#define __DEBUG___H__

#include <debug/debugconsole.h>

#include <debug/debugconsole.h>
#include <debug/consolecolour.h>

#ifdef VERBOSE
#define VINFO(x) INFO(x)
#else
#define VINFO(x) {}
#endif

#ifdef DEBUG
#define INFO(x) {	DebugConsole::GetInstance().LockConsole(); DebugConsole::GetInstance() << ConsoleColour(0x00FF00, 0x000000) << "[INFO]    " << ConsoleColour(0xFFFFFF, 0x000000) << x << '\n'; DebugConsole::GetInstance().UnlockConsole();	}
#else
#define INFO(x) {}
#endif

#ifdef DEBUG
#define WARNING(x) {	DebugConsole::GetInstance().LockConsole(); DebugConsole::GetInstance() << ConsoleColour(0xFF6600, 0x000000) << "[WARN]    " << ConsoleColour(0xFFFFFF, 0x000000) << x << '\n';	DebugConsole::GetInstance().UnlockConsole(); }
#else
#define WARNING(x) {}
#endif

#define FATAL(x) {	DebugConsole::GetInstance().LockConsole(); DebugConsole::GetInstance() << ConsoleColour(0xFF0000, 0x000000) << "\n[ERROR]   " << ConsoleColour(0xFFFFFF, 0x000000) << x << " - System Halted."; DebugConsole::GetInstance().UnlockConsole(); for(;;){} }

#endif
