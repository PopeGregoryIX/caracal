#include <stdint.h>
#include <runtime/cxx.h>

extern "C" void* memcpy( void* dest, void* src, size_t numBytes)
{
	return memorycopy(dest, src, numBytes);
}