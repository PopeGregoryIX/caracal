/*
 * consoleoutput.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#include <debug/consoleoutput.h>
#include <stdint.h>
#include <spinlock.h>

ConsoleOutput::ConsoleOutput( void )
{
	_colour.Foreground = 0xFFFFFF;
	_colour.Background = 0x000000;
}

void ConsoleOutput::PutString(const char *s)
{
	while(*s) PutChar(*s++);
}

void ConsoleOutput::SetColour(uint32_t foreground, uint32_t background)
{
	_colour.Foreground = foreground, _colour.Background = background;
}
