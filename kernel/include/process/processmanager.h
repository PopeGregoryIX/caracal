/*
 * processmanager.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_PROCESS_PROCESSMANAGER_H_
#define KERNEL_INCLUDE_PROCESS_PROCESSMANAGER_H_

#include <archdef.h>
#include <support/templates/list.h>
#include <process/process.h>
#include <process/thread.h>

class ProcessManager
{
	public:
		static ProcessManager& GetInstance( void ){ return instance_; }

		ProcessManager( void );

		void Initialise( arch::processState_t* initialProcessState, arch::threadState_t* initialThreadState,
				arch::threadId_t initialThreadId);

		inline Thread* GetRunningThread(){ return GetRunningThread(Cpu::ProcessorId()); }

		inline Thread* GetRunningThread(size_t processorId)
		{
			//INFO("Processor ID " << (uintptr_t)processorId);
			for(size_t i = 0; i < this->runningThreadCount_; ++i)
			{
				if( runningThreads_[i].processorId == processorId)
					return runningThreads_[i].thread;
			}

			return nullptr;
		}

		Thread* TaskSwitch(Thread* outgoing);
	private:
		typedef struct runningThread
		{
			size_t processorId;
			Thread* thread;
		} runningthread_t;

		arch::processId_t nextId_;
		static ProcessManager instance_;
		List<Process*> processes_;

		size_t runningThreadCount_;
		runningthread_t* runningThreads_;
};


#endif /* KERNEL_INCLUDE_PROCESS_PROCESSMANAGER_H_ */
