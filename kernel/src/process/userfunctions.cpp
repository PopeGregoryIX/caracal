/*
 * userfunctions.cpp
 *
 *  Created on: 25 Jun 2023
 *      Author: mail
 */

#include <archdef.h>
#include <debug/debug.h>
#include <stdint.h>
#include <stddef.h>
#include <process/thread.h>
#include <process/process.h>
#include <process/processmanager.h>
#include <process/userfunctions.h>

UserFunctions UserFunctions::instance_;

Thread* UserFunctions::Halt(Thread* outgoing)
{
    FATAL("Thread ID " << outgoing->GetId() << " on process " << outgoing->GetProcess().GetId() << " has halted the CPU.");
    return outgoing;
}