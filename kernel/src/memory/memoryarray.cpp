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
#include <bootboot.h>

MemoryArray MemoryArray::_instance;

const char* MemoryArray::_memoryType[4] = { "Used", "Free", "ACPI", "MMIO" };

MemoryArray::MemoryArray( void )
: StaticArray((MemoryMapEntry*)_mmap, 0, MMAP_MAX_ENTRIES)
{
    //  pull in the BootBoot memory map so that we can perform allocations
    _count = 0;

    MMapEnt* nextEntry = &(bootboot.mmap);

    while((uintptr_t)nextEntry < (uintptr_t)&bootboot + bootboot.size)
    {
        _mmap[_count++] = {nextEntry->ptr, nextEntry->size & ~0xFULL, (uint8_t)MMapEnt_Type(nextEntry) };
        nextEntry++;
    }
}

void MemoryArray::Align( size_t alignVal)
{
    MemoryArrayIterator i = GetIterator();
    do
    {
        size_t delta = alignVal - (i.Current().base % alignVal);
        
        if(delta != 0)
        {
            i.Current().base+= delta;
            i.Current().size-= delta;
        }

        delta = i.Current().size % alignVal;
        if(delta != 0) i.Current().size -= delta;
    } while(i.MoveNext());
}

void* MemoryArray::Allocate(size_t bytes)
{
    if((bytes % 0x1000) != 0) bytes += (0x1000 - (bytes % 0x1000));

    for(size_t i = Count() - 1; i > 0; --i)
    {
        if(_mmap[i].type == MMAP_FREE && _mmap[i].size >= bytes)
        {
            Insert(i);
            _mmap[i].base = _mmap[i+1].base;
            _mmap[i].size = bytes;
            _mmap[i].type = MMAP_USED;

            _mmap[i+1].base+= bytes;
            _mmap[i+1].size-= bytes;

            return (void*)_mmap[i].base;
        }
    }

    return nullptr;
}

void MemoryArray::Insert(size_t index)
{
    for(size_t i = Count() - 1; i >= index; --i)
    {
        _mmap[i+1] = _mmap[i];
    }

    _mmap[index] = {0,0,0};
    _count++;
}

size_t MemoryArray::GetHighestAddress( void )
{
    size_t returnValue = 0;

    MemoryArrayIterator i = GetIterator();
    do
    {
        if(i.Current().IsFree() && (i.Current().Top() > returnValue)) 
            returnValue = i.Current().Top();
    } while(i.MoveNext());

    return returnValue;
}

size_t MemoryArray::GetTotalFreeMemory( void )
{
    size_t returnValue = 0;
    
    MemoryArrayIterator i = GetIterator();
    do
    {
        if(i.Current().IsFree()) returnValue+= i.Current().size;
    } while(i.MoveNext());

    return returnValue;
}

void MemoryArray::Print()
{
    MemoryArrayIterator i = GetIterator();
    do
    {
        INFO((uint64_t)i.Current().base << " - " << (uint64_t)i.Current().Top() << " : " << _memoryType[i.Current().type]);
    } while(i.MoveNext());
}