#include <archdef.h>
#include <debug/debug.h>
#include <processservices.h>
#include <process/process.h>
#include <process/thread.h>

namespace arch
{
    Thread* ProcessServices::CreateThread(Process* process, uintptr_t entryPoint)
    {
        (void)entryPoint;
        //  TODO: This does not deal with deleted threads!
        size_t threadCount = process->GetThreads().Count();
        uintptr_t newStackLocation = USER_THREAD_STACK - (0x200000 * threadCount);

        INFO("New thread stack will be set up at " << newStackLocation);

        threadState_t* newThreadState = (threadState_t*)(newStackLocation - sizeof(threadState_t));
        newThreadState->rip = entryPoint;

        FATAL("Thread Created");
        return nullptr;
    }
}