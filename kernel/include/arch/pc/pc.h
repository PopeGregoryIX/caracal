#ifndef __PC__H__
#define __PC__H__

#include <arch/machine.h>
#include <cboot.h>

class Glue;

namespace arch
{
    class Pc : public Machine
    {
        public:
            static inline Pc& GetInstance( void ) { return _instance; }

            void Initialise( CBoot& cboot );

        private:
            void AddCurrentCpu();
            static Pc _instance;

        friend class ::Glue;
    };
}

#endif
