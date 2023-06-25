#ifndef __PROCESS_SERVICES_H
#define __PROCESS_SERVICES_H

#include <archdef.h>

class Process;
class Thread;

namespace arch
{
    class ProcessServices
    {
        public:
            static Thread* CreateThread(Process* process, uintptr_t entryPoint);
    };
}

#endif