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

void MemoryArray::Initialise( const MMapEnt* firstEntry, size_t sizeBytes )
{
    _mmap = _memoryMapStaticData;
    //  pull in the BootBoot memory map so that we can perform allocations
    _count = 0;
    _maxCount = MMAP_MAX_ENTRIES;

    MMapEnt* nextEntry = (MMapEnt*)firstEntry;

    while(((uintptr_t)nextEntry - (uintptr_t)firstEntry) <= sizeBytes)
    {
        _mmap[_count++] = {nextEntry->ptr, nextEntry->size & ~0xFULL, (uint8_t)MMapEnt_Type(nextEntry) };
        if(_count > _maxCount) FATAL("MemoryMap size exceeds limit!");
        nextEntry++;
    }
}

void MemoryArray::Initialise( MemoryMapEntry* firstEntry, size_t sizeBytes, size_t limitSizeBytes )
{
    _mmap = firstEntry;
    _count = sizeBytes / sizeof(MemoryMapEntry);
    _maxCount = limitSizeBytes / sizeof(MemoryMapEntry);
}

void MemoryArray::Align()
{
    //  Generally align the memory map to 0x1000 bytes
    for(size_t i = 0; i < Count(); i++)
    {
        size_t delta = 0x1000 - (_mmap[i].base % 0x1000);
        
        if(delta != 0x1000)
        {
            _mmap[i].base+= delta;
            _mmap[i].size-= delta;
        }

        delta = _mmap[i].size % 0x1000;
        if(delta != 0) _mmap[i].size -= delta;
    }
}

uintptr_t MemoryArray::Allocate(size_t bytes)
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

            return _mmap[i].base;
        }
    }

    return 0;
}

uintptr_t MemoryArray::Allocate4k()
{
    size_t bytes = 0x1000;
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

            return _mmap[i].base;
        }
    }

    return 0;
}

uintptr_t MemoryArray::Allocate2M(size_t bytes)
{
    if((bytes % 0x200000) != 0) bytes += (0x200000 - (bytes % 0x200000));

    for(size_t i = Count() - 1; i > 0; --i)
    {
        if(_mmap[i].type == MMAP_FREE && _mmap[i].size >= (bytes + 0x200000))
        {
            uintptr_t delta = 0x200000 - (_mmap[i].base % 0x200000);
            if (delta!= 0x200000)
            {
                Insert(i);
                _mmap[i].base = _mmap[i+1].base;
                _mmap[i].size = delta;
                _mmap[i].type = MMAP_FREE;

                _mmap[i+1].base+= delta;
                _mmap[i+1].size-= delta;
                i++;
            }

            Insert(i);
            _mmap[i].base = _mmap[i+1].base;
            _mmap[i].size = bytes;
            _mmap[i].type = MMAP_USED;

            _mmap[i+1].base+= bytes;
            _mmap[i+1].size-= bytes;
            
            return _mmap[i].base;
        }
    }

    return 0;
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

    for(size_t i = 0; i < Count(); i++)
    {
        if(_mmap[i].IsFree() && (_mmap[i].Top() > returnValue)) 
            returnValue = _mmap[i].Top();
    }

    return returnValue;
}

size_t MemoryArray::GetTotalFreeMemory( void )
{
    size_t returnValue = 0;
    
    for(size_t i = 0; i < Count(); i++)
    {
        if(_mmap[i].IsFree()) returnValue+= _mmap[i].size;
    }

    return returnValue;
}

void MemoryArray::Print( void )
{
    INFO(Count() << " items with a limit of " << _maxCount << " items." );
    for(size_t i = 0; i < Count(); i++)
    {
        if(_mmap[i].IsFree())
        {
            INFO("Free Area " << _mmap[i].base << " - " << _mmap[i].Top());
        }        
        else
        {
            INFO("Used Area " << _mmap[i].base << " - " << _mmap[i].Top());
        }
    }
}