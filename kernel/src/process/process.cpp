#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <process/process.h>
#include <process/processmanager.h>

Process::Process(arch::processId_t id, arch::processState_t state)
: _id(id), _state(state)
{

}