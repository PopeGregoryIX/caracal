#ifndef __CPU__H__
#define __CPU__H__

#include <cboot.h>

class Thread;
class Glue;

namespace arch
{
    class Cpu
    {
        public:
            inline Thread& GetCurrentThread( void ){ return *_currentThread; }

        protected:
            inline void SetCurrentThread(Thread& thread) { _currentThread = &thread; }
            Thread* _currentThread;

        friend Glue;
    };
}

#endif
