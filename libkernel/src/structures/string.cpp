#include <structures/string.h>
#include <cxx.h>

String& String::operator=(const char* s)
{
    Clear();

    _lock.Acquire();
    _count = strlen(s);

    if(_count > 0)
    {
        _string = new char[_count];
        memcpy((void*)_string, (void*)s, _count);
    }
    _lock.Release();
    
    return *this;
}