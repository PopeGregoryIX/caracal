#ifndef _GLUE_H_
#define _GLUE_H_

#include <cboot.h>

namespace arch
{
    class Glue
    {
        public:
            static void EarlyMemorySetup( CBoot& cboot );
            static void APSetup(CBoot& cboot);
    };
}

#endif