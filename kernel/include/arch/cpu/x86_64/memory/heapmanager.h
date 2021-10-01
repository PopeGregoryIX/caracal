#ifndef __HEAPMANAGER_H_
#define __HEAPMANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>

namespace arch
{   
    class HeapManager
    {
        public:
            static uintptr_t RequestKernelHeapBytes(intptr_t bytes);
        private:
    };
}

#endif
