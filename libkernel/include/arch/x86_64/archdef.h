#ifndef __ARCHDEF__H__
#define __ARCHDEF__H__

#include <stdint.h>
#include <stddef.h>
#include <registers.h>

#define MAX_CPU_COUNT   256

typedef uint64_t processId_t;
typedef uint64_t threadId_t;

typedef uintptr_t processInfo_t;        /// in x86_64, this is the address of the PML4
typedef arch::Registers* threadInfo_t;  /// in x86_64, this is the thread stack pointer

#endif
