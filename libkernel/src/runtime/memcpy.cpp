#include <stdint.h>
#include <cxx.h>

extern "C" void* memcpy( void* dest, void* src, size_t numBytes)
{
	return memorycopy(dest, src, numBytes);
}
