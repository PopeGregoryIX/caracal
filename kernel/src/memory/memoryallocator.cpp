#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <memory/memoryallocator.h>

MemoryAllocator::MemoryAllocator( uintptr_t heapStart, uintptr_t heapMax)
: _heapStart(heapStart), _heapMax(heapMax)
{

}
