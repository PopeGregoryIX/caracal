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

void PrintRegisters(const char* name1, uint64_t value1, const char* name2, uint64_t value2);

}

#endif
