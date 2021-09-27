/*
 * debugconsole.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */
#include <debug/debugconsole.h>
#include <stddef.h>
#include <pcscreenfont.h>
#include <support/string.h>

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

void DebugConsole::SetOutputColour(uint32_t foreground, uint32_t background)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->SetColour(foreground, background);
	}
}

void DebugConsole::Cls( void )
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
					_outputDevices[i]->Cls();
	}
}

void DebugConsole::PutBinary(uint64_t h)
{
	char buffer[65];
	PutString("0b");
	PutString((const char*)String::itoa(h, buffer, 2));
}

void DebugConsole::PutChar(const char c)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutChar(c);
	}
}

void DebugConsole::PutDecimal(uint64_t h)
{
	char buffer[21];
	PutString((const char*)String::itoa(h, buffer, 10));
}

void DebugConsole::PutHex(uint64_t h)
{
	char buffer[17];
	PutString("0x");
	PutString((const char*)String::itoa(h, buffer, 16));
}

void DebugConsole::PutString(const char* s)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutString(s);
	}
}
