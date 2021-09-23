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
#include <debug/debug.h>

bool Cpu::IsBsp( void )
{
	return ProcessorId() == bootboot.bspid;
}

unsigned int Cpu::ProcessorId()
{
	volatile uint32_t discard, ebx;
	__cpuid(1, discard, ebx, discard, discard);

	return ((ebx >> 24) & 0xFF);
}

namespace arch
{

}


