#include <stdint.h>
#include <cxx.h>

extern "C" void* memcpy( void* dest, void* src, size_t numBytes)
{
	if((numBytes % 8) == 0)
        return (void*)memorycopy<uint64_t>((uint64_t*)dest, (uint64_t*)src, numBytes / 8);
    else if ((numBytes % 4) == 0)
        return (void*)memorycopy<uint32_t>((uint32_t*)dest, (uint32_t*)src, numBytes / 4);
    else if ((numBytes % 2) == 0)
        return (void*)memorycopy<uint16_t>((uint16_t*)dest, (uint16_t*)src, numBytes / 2);
    else
        return (void*)memorycopy<char>((const char*)dest, (const char*)src, numBytes);
}
