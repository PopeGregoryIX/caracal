#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/heapmanager.h>
#include <debug/debug.h>

namespace arch
{
    uintptr_t HeapManager::RequestKernelHeapBytes(intptr_t bytes)
    {
        FATAL("Extend kernel heap.");
        return bytes;
    }
}