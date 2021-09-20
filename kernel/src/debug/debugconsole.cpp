/*
 * debugconsole.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */
#include <debug/debugconsole.h>
#include <stddef.h>
#include <pcscreenfont.h>

DebugConsole DebugConsole::_instance;

void DebugConsole::AddOutputDevice(ConsoleOutput& device)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] == nullptr)
		{
			_outputDevices[i] = &device;
			break;
		}
	}
}

void DebugConsole::PutChar(const char c)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutChar(c);
	}
}

void DebugConsole::PutString(const char* s)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutString(s);
	}
}
