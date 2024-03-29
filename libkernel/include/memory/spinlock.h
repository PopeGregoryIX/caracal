#ifndef __SPINLOCK_H
#define __SPINLOCK_H

#include <stdint.h>

class Spinlock
{
    public:
        void Acquire( void );

        void Release( void );

    private:
        uintptr_t _lock = 0;
};

#endif