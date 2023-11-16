#include <pc.h>
#include <arch/machine.h>
#include <cboot.h>
#include <object/objectmanager.h>
#include <x86_64.h>
#include <arch/cpuutilities.h>
#include <archdef.h>
#include <arch/glue.h>

namespace arch
{
    Machine& Machine::GetInstance( void ) { return Pc::GetInstance(); }

    Pc Pc::_instance;

    void Pc::Initialise(CBoot& cboot)
    {
        _name = "PC";
        ::ObjectManager::GetInstance().RegisterObject(*this);

        _cpus = (Cpu**)new uintptr_t[MAX_CPU_COUNT];
        memset(_cpus, 0, sizeof(uintptr_t) * MAX_CPU_COUNT);

        //  Add the bootstrap cpu
        ::Glue::AddCurrentCpu();
    }

    void Pc::AddCurrentCpu( void )
    {
        int currentId = CpuUtilities::GetCurrentProcessorId();
        if(currentId > MAX_CPU_COUNT) FATAL("Either non-linear APIC ID's or high CPU count resulted in CPU overflow. Not yet supported");

        _cpus[currentId] = new X86_64(currentId);
    }
}