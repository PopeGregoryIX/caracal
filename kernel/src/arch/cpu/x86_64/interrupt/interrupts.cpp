/*
 * interrupts.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */
#include <interrupt/interrupts.h>
#include <process/processmanager.h>
#include <debug/debug.h>
#include <process/userfunctions.h>

namespace arch
{
	Registers* Interrupts::SoftwareInterrupt(Registers* registers)
	{
		Thread* outgoing = ProcessManager::GetInstance().GetRunningThread();
		Thread* incoming = nullptr;
		INFO("Outgoing Thread: " << outgoing->GetId() << " process " << outgoing->GetProcess().GetId())

		switch (registers->rax)
		{
			case CALL_HALT:
				incoming = UserFunctions::Halt(outgoing);
				break;
			case CALL_YIELD:
				incoming = UserFunctions::Yield(outgoing);
				break;
			default:
				FATAL("Unknown software interrupt " << registers->rax);
				break;
		}

		if(incoming != outgoing)
		{
			outgoing->SaveThreadState(registers);
			registers = incoming->LoadThreadState();
		}

		
		INFO("Incoming Thread: " << incoming->GetId() << " process " << incoming->GetProcess().GetId());
		return registers;
	}
}
