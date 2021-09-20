/*
 * callConstructors.cpp
 *
 *  Created on: 12 May 2013
 *      Author: Adam
 */

#include <runtime/cxx.h>

void callConstructors( void )
{
	/** A typedef for the default constructor function pointer type */
		typedef void (*function_pointer) (void);
		/** Externs to the linker symbols */
		/** Constructors */
		extern function_pointer start_ctors[];
		extern function_pointer end_ctors[];

		if(start_ctors == nullptr || end_ctors == nullptr) return;

		uint64_t numberOfConstructors = (uint64_t)(end_ctors - start_ctors);

		for(uint64_t x = 0; x < numberOfConstructors; x++)
		{
			function_pointer constructor = start_ctors[x];
			constructor();
		}
}


