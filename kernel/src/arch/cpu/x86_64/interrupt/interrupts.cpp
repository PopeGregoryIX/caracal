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
#include <tables/tss.h>
#include <pc.h>

namespace arch
{
	Registers* Interrupts::SoftwareInterrupt(Registers* registers)
	{
		Thread* outgoing = ProcessManager::GetInstance().GetRunningThread();
		Thread* incoming = nullptr;
		VINFO("Outgoing Thread: " << outgoing->GetId() << " stack " << (uintptr_t)registers)
		X86_64& cpu = (X86_64&)(Pc::GetInstance().GetCpu());

		switch (registers->rax)
		{
			case CALL_HALT:
				incoming = UserFunctions::Halt(outgoing);
				break;
			case CALL_YIELD:
				incoming = UserFunctions::GetInstance().Yield(outgoing);
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
		else
			FATAL("incoming == outgoing");
		
		INFO("TSS ID is " << (uintptr_t)cpu.GetTssId());
		//Tss::GetInstance().SetIst1(cpu.GetTssId(), 0);
		VINFO("Incoming Thread: " << incoming->GetId() << " stack " << (uintptr_t)registers)
		return registers;
	}
}
