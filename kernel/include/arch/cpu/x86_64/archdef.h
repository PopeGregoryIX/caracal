/**
 * @file archdef.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains architecture-dependent definitions.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef ARCHDEF_H_
#define ARCHDEF_H_

#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include "x86_64.h"
#include <registers.h>
#include "memory/memorylayout.h"

/**
 * @brief Contains the architecture-dependent portions of the kernel.
 * 
 */
namespace arch
{

#define CPU_CLASS X86_64
#define CPU X86_64::GetInstance()

typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
typedef uint64_t processId_t;
typedef uint64_t threadId_t;

typedef uint64_t processState_t;        //< This will be a value of CR3 for the current process
typedef Registers* threadState_t;       //< This will be a pointer to the top of a stack for the current process.

typedef struct pageTableEntry
{
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user:1;
    uint64_t writeThrough:1;
    uint64_t cacheDisable:1;
    uint64_t accessed:1;
    uint64_t dirty:1;
    uint64_t zero:1;
    uint64_t global:1;
    uint64_t available:3;
    uint64_t page:52;

    inline uint64_t Address() { return page  * 0x1000;  }
} pageTableEntry_t;

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

void PrintRegisters(const char* name1, uint64_t value1, const char* name2, uint64_t value2);

}

#endif
