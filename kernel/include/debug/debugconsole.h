/*
 * debugconsole.h
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_
#define KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_

#include <debug/consoleoutput.h>
#include <debug/consolecolour.h>
#include <stdint.h>
#include <spinlock.h>

class DebugConsole
{
public:
	static DebugConsole& GetInstance( void ) { return _instance; }

	inline void LockConsole( void ){characterLock.Acquire();}
	inline void UnlockConsole( void ){characterLock.Release();}

	void AddOutputDevice(ConsoleOutput& device);
	void SetOutputColour(uint32_t foreground, uint32_t background);
	inline void SetOutputColour(ConsoleColour c) { SetOutputColour(c.Foreground, c.Background); }
	void Cls( void );

	void PutBinary(uint64_t b);
	void PutChar(const char c);
	void PutDecimal(uint64_t d);
	void PutHex(uint64_t h);
	void PutPaddedHex(uint64_t h);
	void PutString(const char* s);

	~DebugConsole(){}

	inline DebugConsole& operator<<(const char c) { PutChar(c); return *this; }
	inline DebugConsole& operator<<(const char* s) { PutString(s); return *this; }
	inline DebugConsole& operator<<(uint64_t i) { PutHex(i); return *this; }
	inline DebugConsole& operator<<(ConsoleColour c) { SetOutputColour(c); return *this; }
protected:
	static DebugConsole _instance;

	static const int MAXOUTPUTDEVICES = 3;
	ConsoleOutput* _outputDevices[MAXOUTPUTDEVICES];
	Spinlock characterLock;
};

#endif /* KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_ */
