/*
 * consoleoutput.h
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_
#define KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_

#include <stdint.h>
#include <debug/consolecolour.h>
#include <memory/spinlock.h>

class ConsoleOutput
{
public:
	ConsoleOutput();

	virtual void PutChar(const char c) = 0;
	virtual void PutString(const char* s);

	virtual void Cls( void ) = 0;

	inline void SetColour(ConsoleColour c) { SetColour(c.Foreground, c.Background); }
	void SetColour(uint32_t foreground, uint32_t background);

	virtual ~ConsoleOutput(){}
protected:
	ConsoleColour _colour;
	Spinlock _lock;
};

#endif /* KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_ */
