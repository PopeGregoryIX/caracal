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

		inline Thread* GetRunningThread(int processorId)
		{
			INFO("Processor ID " << (uintptr_t)processorId);
			for(int i = 0; i < this->runningThreadCount_; ++i)
			{
				if( runningThreads_[i].processorId == processorId)
					return runningThreads_[i].thread;
			}

			return nullptr;
		}
	private:
		typedef struct runningThread
		{
			int processorId;
			Thread* thread;
		} runningthread_t;

		arch::processId_t nextId_;
		static ProcessManager instance_;
		List<Process*> processes_;

		int runningThreadCount_;
		runningthread_t* runningThreads_;
};


#endif /* KERNEL_INCLUDE_PROCESS_PROCESSMANAGER_H_ */
