#ifndef __X86__CPU__H__
#define __X86__CPU__H__

#include <cboot.h>
#include <arch/cpu.h>
#include <gdt.h>
#include <memorylayout.h>

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

            int GetId( void ) { return _id; }

            Gdt& GetGdt( void ) { return _gdt; }
        private:
            X86_64(int id) : _id(id), _gdt(((Gdt*)MEMRANGE_GDT)[id]) {};

            int _id;
            Gdt& _gdt;
            

            friend class Pc;
    };    
}

#endif
