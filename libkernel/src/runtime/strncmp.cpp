#include <stdint.h>
#include <cxx.h>

int strncmp(const char* s1, const char* s2, int maxLength)
{
    int len = (int)strlen(s1);

    if((maxLength != 0) && (len > maxLength)) len = maxLength;

    for(int i = 0; i < maxLength; ++i)
    {
        if(s1[i] != s2[i])
            return s1[i] - s2[i];
    }

    return 0;
}