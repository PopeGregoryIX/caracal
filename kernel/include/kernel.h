#include <stdint.h>

#ifndef _CARACAL_H_
#define _CARACAL_H_

typedef uint64_t handle_t;

extern "C" void kmain(class CBoot* cbootPtr);
extern "C" void _init( void );
extern "C" void _fini( void );

#endif