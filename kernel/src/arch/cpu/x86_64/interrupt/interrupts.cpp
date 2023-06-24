/*
 * interrupts.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <interrupt/interrupts.h>
#include <process/processmanager.h>

namespace arch
{
	Registers* Interrupts::SoftwareInterrupt(Registers* registers)
	{
		auto thread = ProcessManager::GetInstance().GetRunningThread();

		INFO("Software interrupt function " << registers->rax);
		INFO("Outgoing Thread: " << thread->GetId() << " process " << thread->GetProcess().GetId());
		thread->SaveThreadState(registers);


		registers = thread->LoadThreadState();
		INFO("Incoming Thread: " << thread->GetId() << " process " << thread->GetProcess().GetId());
		return registers;
	}
}
