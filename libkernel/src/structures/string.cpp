#include <structures/string.h>
#include <cxx.h>

String& String::Add(const char* s, size_t count)
{
    if(count == 0) return *this;

    _lock.Acquire();

    size_t oldCount = _count;
    const char* oldString = _string;
    _count+= count;

    if(_count > 0)
    {
        _string = new char[_count];
        memcpy((void*)_string, (void*)oldString, oldCount);
        memcpy((void*)(&(_string[oldCount])), (void*)s, count);
    }
    _lock.Release();
    
    delete oldString;

    return *this;
}

bool String::Equals(const char* s, size_t count)
{
    for(size_t i = 0; i < count; ++i)
        if(_string[i] != s[i]) return false;

    return true;
}

String& String::Set(const char* s, size_t count)
{
    Clear();
    if(count == 0) return *this;

    _lock.Acquire();
    _count = count;

    if(_count > 0)
    {
        _string = new char[_count];
        memcpy((void*)_string, (void*)s, _count);
    }
    _lock.Release();
    
    return *this;
}

String String::operator+(const char* s)
{
    String newString = *this;
    newString+= s;
    return newString;
}

String String::operator+(const String& s)
{
    String newString = *this;
    newString+= s;
    return newString;
}

bool String::operator==(const char* s)
{
    if(_string == nullptr && s == nullptr) return true;
    size_t count = strlen(s);
    if(count != _count) return false;

    return this->Equals(s, count);
}

bool String::operator==(const String& s)
{
    if(_string == nullptr && s._string == nullptr) return true;
    if(_count != s._count) return false;
    return this->Equals(s._string, _count);
}