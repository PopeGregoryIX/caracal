#ifndef _GLUE_H_
#define _GLUE_H_

#include <cboot.h>
#include <archdef.h>
#include <structures/string.h>

class Glue
{
    public:
        static void AddCurrentCpu( void );
        
        static void APSetup(CBoot& cboot);

        static void EarlyMemorySetup( CBoot& cboot );

        static void MachineSetup( CBoot& cboot );

        static class Process& CreateProcess( bool supervisor, String name );

        static class Process& GenerateInitialProcess( void );
};

#endif