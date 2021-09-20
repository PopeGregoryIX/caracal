/*
 * consoleoutput.h
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_
#define KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_

class ConsoleOutput
{
public:
	virtual void PutChar(const char c){(void)c;}
	virtual void PutString(const char* s){(void)s;}

	virtual ~ConsoleOutput(){}
};

#endif /* KERNEL_INCLUDE_DEBUG_CONSOLEOUTPUT_H_ */
