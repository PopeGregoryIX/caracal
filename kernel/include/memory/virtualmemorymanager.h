#ifndef __VIRTUALMEMORYMANAGER__H_
#define __VIRTUALMEMORYMANAGER__H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>

class VirtualMemoryManager
{
    private:
        MemoryAllocator _kernelAllocator;

        static VirtualMemoryManager _instance;

    public:
        static inline VirtualMemoryManager& GetInstance( void )    {   return _instance;   }

        VirtualMemoryManager( void );

        inline MemoryAllocator& GetKernelAllocator( void ) { return _kernelAllocator; }

        //  Goes through all memory ranges to check whether we've allocated or not.
        inline bool IsAllocated(uintptr_t address)
        {
            if(_kernelAllocator.IsAllocated(address)) return true;
            return false;
        }
};

#endif