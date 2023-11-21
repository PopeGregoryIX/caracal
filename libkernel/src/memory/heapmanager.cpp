#include <stdint.h>
#include <stddef.h>
#include <memory/heapmanager.h>
#include <debug.h>
#include <memorylayout.h>

namespace arch
{
    uintptr_t HeapManager::RequestHeapBytes(intptr_t bytes)
    {
        _lock.Acquire();
        if(bytes > 0)
            _currentBreakPoint = ((_currentBreakPoint + bytes) > KERNEL_HEAP_MAX) ? KERNEL_HEAP_MAX : (_currentBreakPoint + bytes);
        else if(bytes < 0)
            _currentBreakPoint = ((_currentBreakPoint + bytes) < MEMRANGE_HEAP) ? MEMRANGE_HEAP : (_currentBreakPoint + bytes);
        
        _lock.Release();
        return _currentBreakPoint;
    }
}