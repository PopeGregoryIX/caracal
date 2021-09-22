/*
 * x86_64.cpp
 *
 *  Created on: 22 Sept 2021
 *      Author: Adam
 */

#include<cpu.h>
#include<x86_64.h>
#include<bootboot.h>
#include <cpuid.h>
#include <debug/debugconsole.h>

bool Cpu::IsBsp( void )
{
	return ProcessorId() == bootboot.bspid;
}

unsigned int Cpu::ProcessorId()
{
	volatile uint32_t eax, ebx, ecx, edx;
	__cpuid(1, eax, ebx, ecx, edx);

	return ((ebx >> 24) & 0xFF);
}

namespace arch
{

}


