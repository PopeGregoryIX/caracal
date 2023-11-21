#include <stdint.h>
#include <cxx.h>

int strcmp(const char* s1, const char* s2)
{
    int len = (int)strlen(s1);
    int len2 = (int)strlen(s2);
    if(len != len2) return 1;

    for(int i = 0; i < len; ++i)
    {
        if(s1[i] != s2[i])
            return s1[i] - s2[i];
    }

    return 0;
}