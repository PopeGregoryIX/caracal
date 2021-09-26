/**
 * @file memorymap.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief 
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include <memory/memorymap.h>
#include <bootboot.h>

MemoryMap MemoryMap::_instance;

const char* MemoryMap::_memoryType[4] = { "Used", "Free", "Acpi", "Mmio" };

MemoryMap::MemoryMap( void )
{
    //  pull in the BootBoot memory map so that we can perform allocations
    _mmapEntries = 0;
    MMapEnt* nextEntry = &(bootboot.mmap);

    while((uintptr_t)nextEntry < (uintptr_t)&bootboot + bootboot.size)
    {
        _mmap[_mmapEntries++] = {nextEntry->ptr, nextEntry->size & ~0xFULL, (uint8_t)MMapEnt_Type(nextEntry) };
        nextEntry++;
    }
}

void MemoryMap::Align( size_t alignVal)
{
    for(size_t i = 0; i < _mmapEntries; ++i)
    {
        size_t delta = alignVal - (_mmap[i].base % alignVal);
        
        if(delta != 0)
        {
            _mmap[i].base+= delta;
            _mmap[i].size-= delta;
        }

        delta = _mmap[i].size % alignVal;
        if(delta != 0) _mmap[i].size -= delta;
    }
}

void* MemoryMap::Allocate(size_t bytes)
{
    if((bytes % 0x1000) != 0) bytes += (0x1000 - (bytes % 0x1000));

    for(size_t i = _mmapEntries - 1; i > 0; --i)
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

void MemoryMap::Insert(size_t index)
{
    for(size_t i = _mmapEntries - 1; i >= index; --i)
    {
        _mmap[i+1] = _mmap[i];
    }

    _mmap[index] = {0,0,0};
    _mmapEntries++;
}

size_t MemoryMap::GetHighestAddress( void )
{
    size_t returnValue = 0;

    for(size_t i = 0; i < _mmapEntries; ++i)
        if(_mmap[i].IsFree() && (_mmap[i].Top() > returnValue)) returnValue = _mmap[i].Top();

    return returnValue;
}

size_t MemoryMap::GetTotalFreeMemory( void )
{
    size_t returnValue = 0;

    for(size_t i = 0; i < _mmapEntries; ++i)
        if(_mmap[i].IsFree()) returnValue+= _mmap[i].size;

    return returnValue;
}

void MemoryMap::Print()
{
    for(size_t i = 0; i < _mmapEntries; ++i)
    {
        INFO((uint64_t)_mmap[i].base << " - " << (uint64_t)_mmap[i].Top() << " : " << _memoryType[_mmap[i].type]);
    }
}