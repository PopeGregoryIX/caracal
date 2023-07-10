/*
 * processmanager.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <process/processmanager.h>
#include <process/process.h>
#include <process/thread.h>
#include <processservices.h>

ProcessManager ProcessManager::instance_;

Process& ProcessManager::CreateNewSupervisorProcess( uintptr_t entryPoint )
{
    Process* process = arch::ProcessServices::GetInstance().CreateProcess();
    process->CreateThread( entryPoint );

    _runningProcesses.Add(*process);
    
    return *process;
}

void ProcessManager::IdleTask( void )
{
    INFO("Entered Idle Task");
    for(;;){}
}