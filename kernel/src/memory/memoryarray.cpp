/**
 * @file memoryarray.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Implementation of the MemoryArray class.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include <memory/memoryarray.h>
#include <cboot.h>

const char* MemoryArray::_memoryType[4] = { "Used", "Free", "ACPI", "MMIO" };

MemoryArray::MemoryArray( uint64_t mmap, size_t bytes )
{
    _data = (MemoryMapEntry*)mmap;
    _maxCount = _count = bytes / sizeof(MemoryMapEntry);
    _maxCount++;

    Print();
}

void MemoryArray::Align( size_t alignVal)
{
    for(size_t i = 0; i < Count(); i++)
    {
        size_t delta = alignVal - (_data[i].base % alignVal);
        
        if(delta != 0)
        {
            _data[i].base+= delta;
            _data[i].size-= delta;
        }

        delta = _data[i].size % alignVal;
        if(delta != 0) _data[i].size -= delta;
    }
}

void* MemoryArray::Allocate(size_t bytes)
{
    if((bytes % 0x1000) != 0) bytes += (0x1000 - (bytes % 0x1000));

    for(size_t i = Count() - 1; i > 0; --i)
    {
        if(_data[i].type == MMAP_FREE && _data[i].size >= bytes)
        {
            Insert(i);
            _data[i].base = _data[i+1].base;
            _data[i].size = bytes;
            _data[i].type = MMAP_USED;

            _data[i+1].base+= bytes;
            _data[i+1].size-= bytes;

            return (void*)_data[i].base;
        }
    }

    return nullptr;
}

void MemoryArray::Insert(size_t index)
{
    for(size_t i = Count() - 1; i >= index; --i)
    {
        _data[i+1] = _data[i];
    }

    _data[index] = {0,0,0};
    _count++;
}

size_t MemoryArray::GetHighestAddress( void )
{
    size_t returnValue = 0;

    for(size_t i = 0; i < Count(); i++)
    {
        if(_data[i].IsFree() && (_data[i].Top() > returnValue)) 
            returnValue = _data[i].Top();
    }

    return returnValue;
}

size_t MemoryArray::GetTotalFreeMemory( void )
{
    size_t returnValue = 0;
    
    for(size_t i = 0; i < Count(); i++)
    {
        if(_data[i].IsFree()) returnValue+= _data[i].size;
    }

    return returnValue;
}

void MemoryArray::Print()
{
    for(size_t i = 0; i < Count(); i++)
    {
        INFO((uint64_t)_data[i].base << " - " << (uint64_t)_data[i].Top() << " : " << _memoryType[_data[i].type]);
    }
}