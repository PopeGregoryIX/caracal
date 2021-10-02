#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/heapmanager.h>
#include <debug/debug.h>

namespace arch
{
    uintptr_t HeapManager::_kernelHeapBreak = KERNEL_HEAP;
    uintptr_t HeapManager::RequestKernelHeapBytes(intptr_t bytes)
    {
        if(bytes > 0)
            _kernelHeapBreak = ((_kernelHeapBreak + bytes) > KERNEL_HEAP_MAX) ? KERNEL_HEAP_MAX : (_kernelHeapBreak + bytes);
        else if(bytes < 0)
            _kernelHeapBreak = ((_kernelHeapBreak + bytes) < KERNEL_HEAP) ? KERNEL_HEAP : (_kernelHeapBreak + bytes);
        
        return _kernelHeapBreak;
    }
}