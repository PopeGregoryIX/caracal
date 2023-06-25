/*
 * processmanager.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <process/processmanager.h>
#include <process/process.h>
#include <process/thread.h>

ProcessManager ProcessManager::instance_;

ProcessManager::ProcessManager( void )
: nextId_(1), runningThreadCount_(0), runningThreads_(nullptr)
{

}

void ProcessManager::Initialise( arch::processState_t* initialProcessState, arch::threadState_t* initialThreadState,
		arch::threadId_t initialThreadId)
{
	Process* initProcess = new Process(nextId_++, initialProcessState);
	Thread* initThread = initProcess->AddThread(initialThreadId, initialThreadState);
	processes_.Add(initProcess);

	this->runningThreads_ = new(runningthread_t);
	runningThreads_[0].processorId = Cpu::ProcessorId();
	runningThreads_[0].thread = initThread;

	runningThreadCount_++;
}

Thread* ProcessManager::TaskSwitch(Thread* outgoing)
{
	auto process = GetRunningThread()->GetProcess();

    Thread** currentThread = process.GetThreads().GetFirst();
    Thread* incoming = outgoing;

    while(currentThread != nullptr)
    {
        if(*currentThread != outgoing)
        {
            incoming = *currentThread;
            break;
        }
        currentThread = process.GetThreads().GetNext(currentThread);
    }

	if(incoming != outgoing)
	{
		for(size_t i = 0; i < runningThreadCount_; ++i)
		{
			if(runningThreads_[i].processorId == Cpu::ProcessorId())
			{
				runningThreads_[i].thread = incoming;
				break;
			}
		}
	}

    return incoming;    
}



