/*
 * thread.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_PROCESS_THREAD_H_
#define KERNEL_INCLUDE_PROCESS_THREAD_H_

#include <archdef.h>

class Thread
{
	private:
		arch::threadId_t id_;
		class Process& process_;
		arch::threadState_t* state_;

	public:
		Thread(arch::threadId_t id, class Process& process, arch::threadState_t* state);

		inline class Process& GetProcess( void ) { return process_; }
		inline arch::threadId_t GetId( void ){ return id_; }

		inline void SaveThreadState(arch::threadState_t* state){ state_ = state;}
		inline arch::threadState_t* LoadThreadState() { return state_; }
};


#endif /* KERNEL_INCLUDE_PROCESS_THREAD_H_ */
