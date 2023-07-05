/**
 * @file machine.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief An interface between architecture-dependent and independent sections of the kernel.
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 * This class declaration should contain *only* pure virtuals and statics which are
 * all dealt with by the specific architecture. For example, the Pc-specific
 * instance is in Pc/Pc.h and implements Machine::GetInstance.
 */

#ifndef KERNEL_SRC_ARCH_MACHINE_MACHINE_H_
#define KERNEL_SRC_ARCH_MACHINE_MACHINE_H_

#include <stddef.h>
#include <debug/debugconsole.h>

class Cpu;

class Machine
{
public:
	static Machine& GetInstance( void );

	virtual void AddDefaultConsoleDevices( DebugConsole& ) = 0;

	virtual bool ApBoot( void ) = 0;

	virtual void AcquireLock(uintptr_t* lock) = 0;

	virtual bool Boot( void ) = 0;

	virtual Cpu& GetCpu( void ) = 0;

	virtual Cpu& GetCpu(uintptr_t id) = 0;

	virtual size_t GetCpuCount( void ) = 0;

	virtual void HaltCurrentCore() { while(1); }

	virtual void ReleaseLock( uintptr_t* lock ) = 0;

	virtual ~Machine( void ){}
};

#endif /* KERNEL_SRC_ARCH_MACHINE_MACHINE_H_ */
