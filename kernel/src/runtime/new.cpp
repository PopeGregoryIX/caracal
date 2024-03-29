/**
 * @file new.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains definitions of the new function.
 * @version 0.1
 * @date 2021-10-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stddef.h>
#include <stdint.h>
#include <arch/machine.h>

/**
 * @brief Standard new operator - assumes allocations in the kernel heap.
 * 
 * @param size Size to allocate in bytes.
 * @return void* 
 */
void *operator new(size_t size)
{
	void* returnValue = arch::Machine::GetHeapAllocator().Allocate(size);
	if(returnValue == nullptr) FATAL("NULL memory allocation.");
	return returnValue;
}

/**
 * @brief Array new operator - assumes allocations in the kernel heap.
 * 
 * @param size Size to allocate in bytes.
 * @return void* 
 */
void *operator new[](size_t size)
{
	void* returnValue = arch::Machine::GetHeapAllocator().Allocate(size);
	if(returnValue == nullptr) FATAL("NULL memory allocation.");
	return returnValue;
}
