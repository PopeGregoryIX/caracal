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

		ProcessManager( void ) : _processId(0){}

		Process& CreateNewSupervisorProcess( uintptr_t entryPoint );

		static void IdleTask( void );

		inline uint64_t GetnewProcessId( void ){ return _processId++; }
	private:
		static ProcessManager instance_;
		uint64_t _processId;
		
};


#endif /* KERNEL_INCLUDE_PROCESS_PROCESSMANAGER_H_ */
