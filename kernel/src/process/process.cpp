/*
 * process.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <archdef.h>
#include <process/process.h>
#include <processservices.h>

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

Thread* Process::CreateThread(uintptr_t entry)
{
	// Create a new thread on this process with the given entry point
	Thread* thread = arch::ProcessServices::GetInstance().CreateThread(this, entry);
	threads.Add(thread);
	return thread;
}

