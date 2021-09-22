/*
 * pc.h
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_SRC_ARCH_MACHINE_PC_PC_H_
#define KERNEL_SRC_ARCH_MACHINE_PC_PC_H_

#include <machine.h>

namespace arch
{
	class Pc : public Machine
	{
	public:
		static Pc& GetPcInstance( void ) { return _instance; }

		void AddDefaultConsoleDevices( DebugConsole& console );
	private:
		static Pc _instance;
	};
}


#endif /* KERNEL_SRC_ARCH_MACHINE_PC_PC_H_ */
