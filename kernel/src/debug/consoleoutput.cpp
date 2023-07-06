/*
 * consoleoutput.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#include <debug/consoleoutput.h>
#include <stdint.h>
#include <memory/spinlock.h>

ConsoleOutput::ConsoleOutput( void )
{
	_colour.Foreground = 0xFFFFFF;
	_colour.Background = 0x000000;
}

void ConsoleOutput::PutString(const char *s)
{
	_lock.Acquire();
	while(*s) PutChar(*s++);
	_lock.Release();
}

void ConsoleOutput::SetColour(uint32_t foreground, uint32_t background)
{
	_lock.Acquire();
	_colour.Foreground = foreground, _colour.Background = background;
	_lock.Release();
}
