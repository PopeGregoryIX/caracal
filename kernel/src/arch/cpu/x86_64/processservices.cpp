#include <archdef.h>
#include <debug/debug.h>
#include <processservices.h>
#include <process/process.h>
#include <process/thread.h>
#include <cpu.h>

namespace arch
{
    ProcessServices ProcessServices::instance_;

    Thread* ProcessServices::CreateThread(Process* process, uintptr_t entryPoint)
    {
        INFO("Create new thread");
        (void)entryPoint;
        //  TODO: This does not deal with deleted threads!
        size_t threadCount = process->GetThreads().Count();
        INFO("Count is " << (uintptr_t)threadCount);
        uintptr_t newStackLocation = SUPERVISOR_THREAD_STACK - (0x200000 * threadCount);
        if(newStackLocation % 0x200000) newStackLocation-= newStackLocation % 0x200000;

        INFO("New thread stack will be set up at " << newStackLocation);
        threadStacks_.Add(newStackLocation);
        X86_64::PageInLarge(PAGE_PRESENT | PAGE_WRITE | PAGE_LARGE, newStackLocation - 0x200000);

        threadState_t* newThreadState = (threadState_t*)(newStackLocation - sizeof(threadState_t));
        newThreadState->rip = entryPoint;
        newThreadState->cs = 0x08;
        newThreadState->ds = 0x10;
        newThreadState->es = 0x10;
        newThreadState->userRsp = 0x400000;
        newThreadState->userSs = 0x10;

        Thread* newThread = new Thread(newStackLocation, *process, newThreadState);
        return newThread;
    }
}