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

/**
 * @brief Contains the architecture-dependent portions of the kernel.
 * 
 */
namespace arch
{
#define CPU X86_64::GetInstance()

typedef uint64_t uintptr_t;
typedef int64_t intptr_t;

#define PRINTREGS(n1, v1, n2, v2)   { INFO(ConsoleColour(0x00FF00, 0x000000) << n1 << ConsoleColour(0xFFFFFF, 0x000000) <<  ":\t" << (uint64_t)v1 << "\t\t" <<  ConsoleColour(0x00FF00, 0x000000) << n2 << ":\t" << ConsoleColour(0xFFFFFF, 0x000000) << (uint64_t)v2); }
}

#endif
