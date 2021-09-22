/*
 * cpu.h
 *
 *  Created on: 28 Feb 2013
 *      Author: Adam
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <stddef.h>

class Cpu
{
	public:
		static bool IsBsp( void );
		static unsigned int ProcessorId( void );
};

#endif /* CPU_H_ */
