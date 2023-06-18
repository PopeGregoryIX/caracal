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
#include <memory/virtualmemorymanager.h>
#include <cpu.h>
#include <stddef.h>

/**
 * @brief Standard new operator - assumes allocations in the kernel heap.
 * 
 * @param size Size to allocate in bytes.
 * @return void* 
 */
void *operator new(size_t size)
{
	void* kalloc = VirtualMemoryManager::GetInstance().GetKernelAllocator().Allocate(size);
	if(kalloc == nullptr) FATAL("Null Pointer Allocated in Kernel Space");
	return kalloc;
}

/**
 * @brief Array new operator - assumes allocations in the kernel heap.
 * 
 * @param size Size to allocate in bytes.
 * @return void* 
 */
void *operator new[](size_t size)
{
	void* kalloc = VirtualMemoryManager::GetInstance().GetKernelAllocator().Allocate(size);
	if(kalloc == nullptr) FATAL("Null Pointer Allocated in Kernel Space");
	return kalloc;
}
