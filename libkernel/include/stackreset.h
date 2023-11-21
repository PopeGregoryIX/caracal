#ifndef __STACKRESET__H_
#define __STACKRESET__H_

#include <stdint.h>

extern "C" void __stackReset(uintptr_t newStack);

#endif
