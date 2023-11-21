#ifndef __MEMORYMAP_H
#define __MEMORYMAP_H

#include <stdint.h>
#include <stddef.h>
#include <debug.h>
#include <cxx.h>
#include <bootboot.h>

#define MMAP_MAX_ENTRIES 0x200

#define MMAP_USED     0
#define MMAP_FREE     1
#define MMAP_ACPI     2
#define MMAP_MMIO     3

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
        static MemoryArray& GetInstance( void )   {   return _instance; }

        static inline uintptr_t AllocateMemorySmall( void ) { return _instance.Allocate4k(); }
        static inline uintptr_t AllocateMemoryLarge( void ) { return _instance.Allocate2M(0x200000); }

        void Initialise( const MMapEnt* firstEntry, size_t sizeBytes );

        void Initialise( MemoryMapEntry* mmap, size_t sizeBytes, size_t limitSizeBytes );

        void Align();

        uintptr_t Allocate(size_t bytes);

        uintptr_t Allocate4k();

        uintptr_t Allocate2M(size_t bytes);

        void Insert(size_t index);

        void Print( void );

        size_t GetTotalFreeMemory( void );

        size_t GetHighestAddress( void );

        MemoryMapEntry* GetFirst( void ) { return (MemoryMapEntry*)_mmap; }

        MemoryMapEntry* GetNext(MemoryMapEntry* current)
        {
            uintptr_t index = ((uintptr_t)current - (uintptr_t)_mmap) / sizeof(MemoryMapEntry);
            if((index + 1) >= this->Count())
                return nullptr;
            else
                return &(_mmap[index + 1]);
        }

        inline size_t Count( void ) { return _count; }

        inline size_t Size( void ) { return _count * sizeof(MemoryMapEntry); }
    private:
        static MemoryArray _instance;
        MemoryMapEntry* _mmap;
        size_t _maxCount;
        size_t _count;
        static const char* _memoryType[4];
        MemoryMapEntry _memoryMapStaticData[MMAP_MAX_ENTRIES];
};

#endif