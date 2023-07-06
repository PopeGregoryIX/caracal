#include <memory/spinlock.h>
#include <machine.h>
#include <archdef.h>

void Spinlock::Acquire( void ) { Machine::GetInstance().AcquireLock(&_lock); }

void Spinlock::Release( void ) { Machine::GetInstance().ReleaseLock(&_lock); }