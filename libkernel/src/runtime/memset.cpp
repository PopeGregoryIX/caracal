#include <stdint.h>
#include <cxx.h>

extern "C" void * memset( void* ptr, int value, size_t num)
{
	char* p = (char*)ptr;
	while(num--) {	*p = (char)value; p++;}
	return ptr;

}
