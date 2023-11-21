#include <debug/consoleoutput.h>
#include <stdint.h>
#include <structures/string.h>

ConsoleOutput::ConsoleOutput( void )    {	_colour.Foreground = 0xFFFFFF;	_colour.Background = 0x000000;  }

void ConsoleOutput::PutString(const char *s)    {	while(*s) PutChar(*s++);    }

void ConsoleOutput::PutString(const String& s){    for(size_t i = 0; i < s.Length(); i++) PutChar(s[i]); }

void ConsoleOutput::SetColour(uint32_t foreground, uint32_t background) {	_colour.Foreground = foreground, _colour.Background = background;   }