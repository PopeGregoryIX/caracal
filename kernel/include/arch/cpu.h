#ifndef __CPU__H__
#define __CPU__H__

#include <cboot.h>

class Thread;

namespace arch
{
    class Cpu
    {
        public:
            inline Thread& GetCurrentThread( void ){ return *_currentThread; }

        private:
            Thread* _currentThread;
    };
}

#endif
