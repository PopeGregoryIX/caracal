#include <stdint.h>
#include <cxx.h>

size_t strlen(const char* s)
{
	char* str = (char*)s;
	size_t i = 0;
	while(*str++) i++;
	return i;
}