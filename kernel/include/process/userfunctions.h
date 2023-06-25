/*
 * userfunctions.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_
#define KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_

#include <process/thread.h>

#define CALL_HALT		0x00000000
#define CALL_YIELD		0x00000001

class UserFunctions
{
    public:
        static Thread* Halt(Thread* outgoing);
        static Thread* Yield(Thread* outgoing);
};


#endif /* KERNEL_INCLUDE_PROCESS_USERFUNCTIONS_H_ */
