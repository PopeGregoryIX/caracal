#include <debug/debugconsole.h>
#include <debug/lfbconsoleoutput.h>
#include <cxx.h>

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

void DebugConsole::Cls( void )
{
	_stringLock.Acquire();
	_characterLock.Acquire();
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
					_outputDevices[i]->Cls();
	}
	_stringLock.Release();
	_characterLock.Release();
}

void DebugConsole::PutBinary(uint64_t h)
{
	_stringLock.Acquire();
	char buffer[65];
	PutString("0b");
	PutString((const char*)itoa(h, buffer, 2));
	_stringLock.Release();
}

void DebugConsole::PutChar(char c)
{
	_characterLock.Acquire();
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutChar(c);
	}
	_characterLock.Release();
}

void DebugConsole::PutDecimal(uint64_t h)
{
	_stringLock.Acquire();
	char buffer[21];
	PutString((const char*)itoa(h, buffer, 10));
	_stringLock.Release();
}

void DebugConsole::PutHex(uint64_t h)
{
	_stringLock.Acquire();
	char buffer[17];
	PutString("0x");
	PutString((const char*)itoa(h, buffer, 16));
	_stringLock.Release();
}

void DebugConsole::PutPaddedHex(uint64_t h)
{
	_stringLock.Acquire();
	char buffer[17];
	PutString("0x");
	itoa(h, buffer, 16);
	size_t len = strlen(buffer);

	for(size_t i = 0; i < 16-len; ++i) PutChar('0');

	PutString((const char*)buffer);
	_stringLock.Release();
}

void DebugConsole::PutString(const char* s)
{
	for(int i = 0; i < MAXOUTPUTDEVICES; ++i)
	{
		if(_outputDevices[i] != nullptr)
			_outputDevices[i]->PutString(s);
	}
}