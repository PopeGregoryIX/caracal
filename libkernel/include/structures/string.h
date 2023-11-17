#ifndef __LIBKERNEL__STRING_H__
#define __LIBKERNEL__STRING_H__

#include <stddef.h>
#include <stdint.h>
#include <memory/spinlock.h>
#include <cxx.h>

class String
{
    public:
        String( void ) : _string(nullptr), _count(0) {}
        String( const char* s) : _string(nullptr), _count(0) { (*this) = s; }
        String( const char* s, size_t count) : _string(nullptr), _count(0) { (*this).Set(s, count); }
        String( const String& s) : _string(nullptr), _count(0) { (*this) = s; }

        ~String( void ) { Clear(); }

        inline void Clear( void ) { _lock.Acquire(); if(_string != nullptr) delete _string; _string = nullptr; _count = 0; _lock.Release(); }

        bool Equals( const char* s, size_t count);

        inline bool IsEmpty( void ) const { return (Length() == 0); }

        inline size_t Length( void ) const { return _count; }

        String& Add(const char* s, size_t count);

        String& Set(const char* s, size_t count);

        String& operator=(const char* s) { return Set(s, strlen(s)); }
        inline String& operator=(const String& s) { return Set(s._string, s._count); }

        inline String& operator+=(const char* s) { return this->Add(s, strlen(s)); }
        inline String& operator+=(const String& s) { return this->Add(s._string, s._count); }

        String operator+(const char* s);
        String operator+(const String& s);

        bool operator==(const char* s);
        bool operator==(const String& s);

        inline bool operator!=(const char* s) { return !(*this == s); }
        inline bool operator!=(const String& s) { return !(*this == s); }

        inline char operator[](size_t index) const { return _string[index]; }
    private:
        Spinlock _lock;
        const char* _string;
        size_t _count;
};

#endif
