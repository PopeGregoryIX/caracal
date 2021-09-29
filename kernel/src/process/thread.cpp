#include <stdint.h>
#include <stddef.h>
#include <archdef.h>
#include <process/thread.h>

Thread::Thread(arch::threadId_t id, arch::threadState_t state)
: _id(id), _state(state)
{

}