#ifndef __X86__CPU__H__
#define __X86__CPU__H__

#include <cboot.h>
#include <arch/cpu.h>

namespace arch
{    
    class X86_64 : public Cpu
    {
        public: 
            // Boot and Setup Routines

            static void APSetup(CBoot& cboot);

            /**
             * \brief Load the Interrupt Descriptor Table
             */ 
            static void IdtSetup( void );

            /**
             * \brief Set up the page frame allocator to allow physical memory allocation.
             */
            static void PageFrameAllocationSetup(CBoot& cboot);

            /**
             * \brief Load paging structures and lay out initial virtual memory.
             */
            static void PagingSetup(CBoot& cboot);
    };    
}

#endif
