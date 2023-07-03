#ifndef __PROCESS_SERVICES_H
#define __PROCESS_SERVICES_H

#include <archdef.h>
#include <support/templates/list.h>

class Process;
class Thread;

namespace arch
{
    class ProcessServices
    {
        public:
            inline static ProcessServices& GetInstance(){ return _instance; }

            Thread* CreateThread(Process* process, uintptr_t entryPoint);
        private:
            static ProcessServices _instance;

            List<uintptr_t> _threadStacks;
    };
}

#endif