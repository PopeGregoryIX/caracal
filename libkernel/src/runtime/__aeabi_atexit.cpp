/*
 * __aeabi_atexit.cpp
 *
 *  Created on: 28 Feb 2013
 *      Author: Adam
 */
#include <cxx.h>

extern "C" int __aeabi_atexit(void* object, void (*destroyer)(void*), void* dso_handle)
{
	return __cxa_atexit(destroyer, object, dso_handle);
}


