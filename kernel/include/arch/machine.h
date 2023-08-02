#ifndef __MACHINE__H__
#define __MACHINE__H__

#include <memory/memoryallocator.h>
#include <memory/heapmanager.h>

namespace arch
{
    class Machine
    {
        public:

            inline static MemoryAllocator& GetHeapAllocator( void ) {   return _heapAllocator;  }

            inline static HeapManager& GetHeapManager( void ) { return _kernelHeapManager; }
        private:
            static MemoryAllocator _heapAllocator;
            static HeapManager _kernelHeapManager;
    };
}

#endif
