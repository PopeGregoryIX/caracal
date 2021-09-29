/*
 * pc.h
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_SRC_ARCH_MACHINE_PC_PC_H_
#define KERNEL_SRC_ARCH_MACHINE_PC_PC_H_

#include <machine.h>
#include <process/process.h>
#include <process/thread.h>

namespace arch
{
	class Pc : public Machine
	{
	public:
		static Pc& GetPcInstance( void ) { return _instance; }

		bool Boot( void );

		void AddDefaultConsoleDevices( DebugConsole& console );

		void CreateInitialProcessSpace( void );

		void HaltCurrentCore()	{	asm("cli\nhlt");	}
	private:
		static Pc _instance;

		static Process _initialProcess;
		static Thread _initialThread;
	};
}


#endif /* KERNEL_SRC_ARCH_MACHINE_PC_PC_H_ */
