/*
 * debugconsole.h
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_
#define KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_

#include <debug/consoleoutput.h>

class DebugConsole
{
public:
	static DebugConsole& GetInstance( void ) { return _instance; }

	void AddOutputDevice(ConsoleOutput& device);

	void PutChar(const char c);

	void PutString(const char* s);

	~DebugConsole(){}
protected:
	static DebugConsole _instance;

	static const int MAXOUTPUTDEVICES = 3;
	ConsoleOutput* _outputDevices[MAXOUTPUTDEVICES];
};

#endif /* KERNEL_INCLUDE_DEBUG_DEBUGCONSOLE_H_ */
