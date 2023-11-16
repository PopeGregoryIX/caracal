#ifndef __MACHINE__H__
#define __MACHINE__H__

#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>
#include <cboot.h>
#include <object/kernelobject.h>

namespace arch
{
    class Machine : public KernelObject
    {
        public:
            static Machine& GetInstance( void );

            inline static MemoryAllocator& GetHeapAllocator( void ) {   return _heapAllocator;  }

            inline static HeapManager& GetHeapManager( void ) { return _kernelHeapManager; }
        protected:
            class Cpu** _cpus;
        private:
            static MemoryAllocator _heapAllocator;
            static HeapManager _kernelHeapManager;
    };
}

#endif
