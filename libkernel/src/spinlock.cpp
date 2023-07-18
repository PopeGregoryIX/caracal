#include <spinlock.h>

extern "C" void __acquireLock(uintptr_t* lock);
extern "C" void __releaseLock(uintptr_t* lock);

void Spinlock::Acquire( void ) { __acquireLock(&_lock); }

void Spinlock::Release( void ) { __releaseLock(&_lock); }