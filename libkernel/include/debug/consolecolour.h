/*
 * consolecolour.h
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_DEBUG_CONSOLECOLOUR_H_
#define KERNEL_INCLUDE_DEBUG_CONSOLECOLOUR_H_

#include <stdint.h>

struct ConsoleColour
{
	uint32_t Foreground;
	uint32_t Background;

	ConsoleColour() : Foreground(0), Background(0){}

	ConsoleColour(uint32_t fg, uint32_t bg)
	: Foreground(fg), Background(bg) {}
};


#endif /* KERNEL_INCLUDE_DEBUG_CONSOLECOLOUR_H_ */
