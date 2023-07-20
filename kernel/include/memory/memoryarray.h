#ifndef __MEMORYMAP_H
#define __MEMORYMAP_H

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <bootboot.h>
#include <debug/debug.h>

#define MMAP_MAX_ENTRIES 0x200

struct MemoryMapEntry
{
    uintptr_t base;
    uintptr_t size;
    uint8_t type;
    bool IsFree() {   return (type & 0xF) == 1; }
    uintptr_t Top() { return base + size; }
};



class MemoryArray
{
    public:
        MemoryArray( uint64_t mmap, size_t bytes );

        void Align(size_t alignVal);

        void* Allocate(size_t bytes);

        void Insert(size_t index);

        void Print( void );

        size_t GetTotalFreeMemory( void );

        size_t GetHighestAddress( void );

        MemoryMapEntry* GetFirst( void ) { return _data; }

        MemoryMapEntry* GetNext(MemoryMapEntry* current)
        {
            uintptr_t index = ((uintptr_t)current - (uintptr_t)_data) / sizeof(MemoryMapEntry);
            if((index + 1) >= this->Count())
                return nullptr;
            else
                return &(_data[index + 1]);
        }

        inline virtual  size_t Count( void ) { return _count; }
    private:
        MemoryMapEntry* _data;
        size_t _maxCount;
        size_t _count;
        static const char* _memoryType[4];
};

#endif
