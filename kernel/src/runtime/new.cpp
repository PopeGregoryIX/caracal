/*
 * new.cpp
 *
 *  Created on: Jun 21, 2012
 *      Author: Adam
 */

//#include <heapmanager.h>
#include <cpu.h>
#include <stddef.h>

void *operator new(size_t /*size*/)
{
	return (void*)0;
	//(void)size;
	//return HeapManager::getInstance().allocate(size);
}

void *operator new[](size_t size)
{
	return (void*)0;
	(void)size;
	//return HeapManager::getInstance().allocate(size);
}
