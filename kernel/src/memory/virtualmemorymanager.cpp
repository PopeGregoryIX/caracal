#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/virtualmemorymanager.h>
#include <memory/memoryallocator.h>

VirtualMemoryManager VirtualMemoryManager::_instance;

VirtualMemoryManager::VirtualMemoryManager( void )
: _kernelAllocator(KERNEL_HEAP_INCREMENT)
{}

void VirtualMemoryManager::Intialise( void )
{
    
}