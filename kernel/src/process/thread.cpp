/*
 * thread.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#include <process/thread.h>

Thread::Thread(arch::threadId_t id, Process& process, arch::threadState_t* state)
: id_(id), process_(process), state_(state)
{
}


