#ifndef __PROCESS__H__
#define __PROCESS__H__

#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <process/thread.h>

class Process
{
    private:
        arch::processId_t _id;
        arch::processState_t _state;
    public:
        Process(arch::processId_t id, arch::processState_t state);
};

#endif
