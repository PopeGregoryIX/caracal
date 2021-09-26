#ifndef __MEMORYMAP_H
#define __MEMORYMAP_H

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <bootboot.h>
#include <debug/debug.h>

class MemoryMap
{
public:
    static MemoryMap& GetInstance( void )   {   return _instance; }

    MemoryMap( void );

    void Align(size_t alignVal);

    void* Allocate(size_t bytes);

    void Insert(size_t index);

    void Print( void );

    size_t GetTotalFreeMemory( void );

    size_t GetHighestAddress( void );
private:
    static MemoryMap _instance;

    static const char* _memoryType[4];

    struct MemoryMapEntry
    {
        uintptr_t base;
        uintptr_t size;
        uint8_t type;
        bool IsFree() {   return (type & 0xF) == 1; }
        uintptr_t Top() { return base + size; }
    };

    size_t _mmapEntries;
    MemoryMapEntry _mmap[0x200];
};

#endif
