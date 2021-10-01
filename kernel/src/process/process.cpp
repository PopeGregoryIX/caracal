#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <process/process.h>
#include <process/processmanager.h>

Process::Process(arch::processId_t id, arch::processState_t state)
: _id(id), _state(state), _mainThread((Thread&)(*(Thread*)0))
{
    //  TODO: Create a new thread via arch;
}

Process::Process(arch::processId_t id, arch::processState_t state, Thread& mainThread)
: _id(id), _state(state), _mainThread(mainThread)
{
    
}