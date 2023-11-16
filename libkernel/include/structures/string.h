#ifndef __LIBKERNEL__STRING_H__
#define __LIBKERNEL__STRING_H__

#include <stddef.h>
#include <stdint.h>
#include <memory/spinlock.h>

class String
{
    public:
        String( void ) : _string(nullptr), _count(0) {}

        inline void Clear( void ) { _lock.Acquire(); if(_string != nullptr) delete _string; _string = nullptr; _count = 0; _lock.Release(); }

        inline bool IsEmpty( void ) const { return (Length() == 0); }

        inline size_t Length( void ) const { return _count; }

        String& operator=(const char* s);
        
        inline char operator[](size_t index) const { return _string[index]; }
    private:
        Spinlock _lock;
        const char* _string;
        size_t _count;
};

#endif
