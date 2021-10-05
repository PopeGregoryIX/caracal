#ifndef __CONTAINER_H
#define __CONTAINER_H

#include <stdint.h>
#include <stddef.h>
#include "iterator.h"

template<class T>
class Container
{
protected:
    size_t _count;

public:
    inline Container() : _count(0){}

    inline virtual size_t Bytes( void ) { return _count * sizeof(T); }

    inline virtual  size_t Count( void ) { return _count; }

    virtual T* GetFirst( void ) = 0;

    virtual T* GetNext(T* current) = 0;
};

#endif