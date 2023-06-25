/**
 * @file cpu.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Generic pure virtual Cpu class.
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <stddef.h>

class Cpu
{
	public:
		static bool IsBsp( void );
		static unsigned int CurrentProcessorId( void );

		virtual uintptr_t GetId( void ) = 0;
};

#endif /* CPU_H_ */
