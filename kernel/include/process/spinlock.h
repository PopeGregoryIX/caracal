#ifndef __SPINLOCK_H
#define __SPINLOCK_H

#include <archdef.h>
#include <machine/machine.h>

class Spinlock
{
    public:
        inline void Acquire( void ) { Machine::GetInstance().AcquireLock(&_lock); }

        inline void Release( void ) { Machine::GetInstance().ReleaseLock(&_lock); }

    private:
        uintptr_t _lock = ATOMIC_FLAG_INIT;
};

#endif