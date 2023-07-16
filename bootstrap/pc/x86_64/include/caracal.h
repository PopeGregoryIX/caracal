#ifndef _CARACAL_H_
#define _CARACAL_H_

#include <stdint.h>

extern "C" void bmain();
extern "C" void _init( void );
extern "C" void _fini( void );

typedef struct pageDirectory
{
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user:1;
    uint64_t writeThrough:1;
    uint64_t cacheDisable:1;
    uint64_t accessed:1;
    uint64_t dirty:1;
    uint64_t size:1;
    uint64_t global:1;
    uint64_t available:3;
    uint64_t page:52;

    inline uint64_t Address() { return page  * 0x1000;  }
    inline void Set(uint64_t Address, bool pres, bool write, bool large = false)
    {
        uint64_t* tmp = (uint64_t*)this;
        *tmp = (Address & ~0xFFF);
        present = (pres) ? 1 : 0;
        writable = (write) ? 1 : 0;
        user = 0;
        writeThrough = 0;
        cacheDisable = 0;
        accessed = 0;
        dirty = 0;
        size = (large) ? 1 : 0;
        global = 0;
        available = 0;
    }    
} pageDirectoryEntry_t;

#endif