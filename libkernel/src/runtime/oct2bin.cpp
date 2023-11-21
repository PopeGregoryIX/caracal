#include <stddef.h>
#include <stdint.h>
#include <cxx.h>

size_t octToBin(unsigned char *str, int size) 
{
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}