#ifndef __ARRAY_H_
#define __ARRAY_H

#include "container.h"
#include "iterator.h"
#include <debug/debug.h>

template<class T>
class StaticArray : public Container<T>
{
protected:
    T*  _data;
    size_t _maxCount;
public:
    StaticArray(T* data, size_t initialCount, size_t maxCount)
    : _data(data), _maxCount(maxCount)
    {
        this->_count = initialCount;
    }

    T* GetFirst( void ) { return _data; }

    T* GetNext(T* current)
    {
        uintptr_t index = ((uintptr_t)current - (uintptr_t)_data) / sizeof(T);
        if((index + 1) >= this->Count())
            return nullptr;
        else
            return &(_data[index + 1]);
    }

    virtual inline Iterator<StaticArray, T> GetIterator( void ) { return Iterator<StaticArray, T>(*this);   } 

    inline size_t MaxBytes( void ) { return _maxCount * sizeof(T); }
};

#endif
