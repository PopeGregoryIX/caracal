/*
 * process.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_PROCESS_PROCESS_H_
#define KERNEL_INCLUDE_PROCESS_PROCESS_H_

#include <archdef.h>
#include <support/templates/list.h>
#include <process/thread.h>

class Process
{
	private:
		arch::processId_t id_;
		arch::processState_t* state_;
		List<Thread*> threads;

	public:
		Process(arch::processId_t id, arch::processState_t* state);

		Thread* AddThread(arch::threadId_t threadId, arch::threadState_t* state);
		Thread* CreateThread(uintptr_t entry);

		inline arch::processId_t GetId( void ) { return id_; }
		inline List<Thread*>& GetThreads() { return threads; }
};


#endif /* KERNEL_INCLUDE_PROCESS_PROCESS_H_ */
