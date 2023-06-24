/*
 * process.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <archdef.h>
#include <process/process.h>

Process::Process(arch::processId_t id, arch::processState_t* state)
: id_(id), state_(state)
{

}

Thread* Process::AddThread(arch::threadId_t threadId, arch::threadState_t* threadState)
{
	Thread* thread = new Thread(threadId, *this, threadState);
	threads.Add(thread);
	return thread;
}



