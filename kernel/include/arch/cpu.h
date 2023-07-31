#ifndef __CPU__H__
#define __CPU__H__

#include <cboot.h>

namespace arch
{
    class Cpu
    {
        public:
            static void EarlyMemorySetup( CBoot& cboot );
            static void APSetup(CBoot& cboot);

        private:
            
    };
}

#endif
