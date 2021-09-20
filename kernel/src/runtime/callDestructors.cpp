/*
 * callDestructors.cpp
 *
 *  Created on: 12 May 2013
 *      Author: Adam
 */

#include <runtime/cxx.h>

void callDestructors( void )
{
	typedef void (*function_pointer) (void);
	/** Externs to the linker symbols */
	extern function_pointer start_dtors[];
	extern function_pointer end_dtors[];

	uint32_t numberOfDestructors = (uint32_t)(end_dtors - start_dtors);

	for(uint32_t x = numberOfDestructors - 1; x > 0; x--)
	{
		function_pointer destructor = start_dtors[x];
		destructor();
	}
}


