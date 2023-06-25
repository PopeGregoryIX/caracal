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
            inline static ProcessServices& GetInstance(){ return instance_; }

            Thread* CreateThread(Process* process, uintptr_t entryPoint);
        private:
            static ProcessServices instance_;

            List<uintptr_t> threadStacks_;
    };
}

#endif