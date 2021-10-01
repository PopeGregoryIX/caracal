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
        Thread& _mainThread;

    public:
        Process(arch::processId_t id, arch::processState_t state);

        Process(arch::processId_t id, arch::processState_t state, Thread& mainThread);

        inline arch::processId_t GetId( void ) { return _id; }

        inline arch::processState_t GetState( void ) { return _state; }
        inline void SetState(arch::processState_t state) { _state = state; }

        inline Process& operator=(const Process& process)
        {
            _id = process._id;
            _state = process._state;
            _mainThread = process._mainThread;

            return *this;
        }
};

#endif
