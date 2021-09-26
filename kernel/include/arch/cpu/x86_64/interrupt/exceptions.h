/**
 * @file exceptions.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Exception handling for X86_64 CPU's
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <stdint.h>
#include <stddef.h>
#include <tables/idt.h>

namespace arch
{
    

    class Exceptions
    {
        private:
        static const char* _ExceptionNames[32];

        static Registers* DefaultHandler(Registers* regs);
    };
}

#endif
