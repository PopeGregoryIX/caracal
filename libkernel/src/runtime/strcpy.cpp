#include <cxx.h>
#include <stdint.h>

char* strcpy(char* dest, const char* src)
{
	int len = (int)(strlen(src) + 1);
	if(dest > src)
		while(--len >=0) dest[len] = src[len];
	else
		for(int i=0; i < len; ++i) dest[i] = src[i];

	return dest;
}