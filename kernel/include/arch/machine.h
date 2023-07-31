#ifndef __MACHINE__H__
#define __MACHINE__H__

#include <memory/memoryallocator.h>

namespace arch
{
    class Machine
    {
        public:

            inline static MemoryAllocator& GetHeapAllocator( void ) {   return _heapAllocator;  }
        private:
            static MemoryAllocator _heapAllocator;
    };
}

#endif
