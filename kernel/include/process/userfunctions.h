/*
 * userfunctions.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_
#define KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_

#include <process/thread.h>
#include <process/processmanager.h>

#define CALL_HALT		0x00000000
#define CALL_YIELD		0x00000001

typedef void (*syscall_function_t)(uintptr_t);

class UserFunctions
{
    public:
        static inline UserFunctions& GetInstance( void ) { return instance_; }
        inline void Initialise(syscall_function_t syscallFunction) { syscall_ = syscallFunction; }
        inline void DoSyscall(uintptr_t function){ syscall_(function);}


        static Thread* Halt(Thread* outgoing);
        inline Thread* Yield(Thread* outgoing) { return outgoing; }

    private:
        static UserFunctions instance_;
        syscall_function_t syscall_;
};


#endif /* KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_ */
