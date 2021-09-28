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
    #define EXCEPTION_PAGE_FAULT    0x0E

    /**
     * @brief Installs and handles CPU exceptions. Works closely with Idt.
     */
    class Exceptions
    {
        private:
            static const char* _ExceptionNames[32]; ///< Names of exceptions - used in the unhandled exception window.

            /**
             * @brief A handler which is executed if no other handler is installed.
             * 
             * @param regs Saved machine state when the handler was invoked.
             * @return Registers* Machine state to load following the exception.
             */
            static Registers* DefaultHandler(Registers* registers);

            static void DumpCore(Registers* registers);
        public:
            static Registers* PageFaultExceptionHandler(Registers* registers);
    };
}

#endif
