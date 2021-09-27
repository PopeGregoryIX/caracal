#ifndef __MEMORYMAP_H
#define __MEMORYMAP_H

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <bootboot.h>
#include <debug/debug.h>
#include <support/templates/iterator.h>
#include <support/templates/staticarray.h>

#define MMAP_MAX_ENTRIES 0x200

struct MemoryMapEntry
{
    uintptr_t base;
    uintptr_t size;
    uint8_t type;
    bool IsFree() {   return (type & 0xF) == 1; }
    uintptr_t Top() { return base + size; }
};

typedef Iterator<StaticArray<MemoryMapEntry>, MemoryMapEntry> MemoryArrayIterator;

class MemoryArray : public StaticArray<MemoryMapEntry>
{
public:
    static MemoryArray& GetInstance( void )   {   return _instance; }

    MemoryArray( void );

    void Align(size_t alignVal);

    void* Allocate(size_t bytes);

    void Insert(size_t index);

    void Print( void );

    size_t GetTotalFreeMemory( void );

    size_t GetHighestAddress( void );

    inline MemoryArrayIterator GetIterator( void ) { return MemoryArrayIterator(*this); }
private:
    static MemoryArray _instance;

    static const char* _memoryType[4];
    MemoryMapEntry _mmap[MMAP_MAX_ENTRIES];
};

#endif
