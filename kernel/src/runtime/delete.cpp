/**
 * \file delete.cpp
 * \date 26 Oct 2011
 * \author Adam Jones (AJSoft)
 * \brief Defines the delete operator override for the kernel.
 */

#include <memory/virtualmemorymanager.h>
#include <cpu.h>
#include <stddef.h>

void operator delete(void *p)
{
	VirtualMemoryManager::GetInstance().GetKernelAllocator().Free(p);
}

void operator delete[](void *p)
{
	VirtualMemoryManager::GetInstance().GetKernelAllocator().Free(p);
}



