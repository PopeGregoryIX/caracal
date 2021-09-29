#ifndef __THREAD__H__
#define __THREAD__H__

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <process/thread.h>

class Thread
{
    private:
        arch::threadId_t _id;
        arch::threadState_t _state;
    public:
        Thread(arch::threadId_t id, arch::threadState_t state);
};

#endif
