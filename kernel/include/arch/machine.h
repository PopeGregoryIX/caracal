#ifndef __MACHINE__H__
#define __MACHINE__H__

#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>
#include <cboot.h>
#include <object/kernelobject.h>
#include <arch/cpuutilities.h>

namespace arch
{
    class Cpu;

    class Machine : public KernelObject
    {
        public:
            static Machine& GetInstance( void );

            inline Cpu& GetCurrentCpu( void ) { return *_cpus[CpuUtilities::GetCurrentProcessorId()]; }

            inline static MemoryAllocator& GetHeapAllocator( void ) {   return _heapAllocator;  }

            inline static HeapManager& GetHeapManager( void ) { return _kernelHeapManager; }
        protected:
            Cpu** _cpus;

        private:
            static MemoryAllocator _heapAllocator;
            static HeapManager _kernelHeapManager;
    };
}

#endif
