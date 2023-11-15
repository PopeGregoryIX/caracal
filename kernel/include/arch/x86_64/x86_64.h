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
            static void IdtSetup( void );
            static void PageFrameAllocationSetup(CBoot& cboot);
            static void PagingSetup(CBoot& cboot);
    };    
}

#endif
