#include <stdint.h>
#include <stddef.h>
#include <cpuutilities.h>
#include <memorylayout.h>
#include <gdt.h>
#include <cboot.h>
#include <caracal.h>
#include <debug.h>

namespace arch
{
    void SetupArchitectureStructures( void )
    {
        Gdt* gdt = (Gdt*)MEMRANGE_GDT;

        if(CpuUtilities::IsBsp())
        {
            CpuUtilities::EnsureMemoryAccessible(MEMRANGE_GDT, sizeof(Gdt) * CpuUtilities::GetProcessorCount(), CARACAL_MEMORY_KERNEL_DATA);

            for(int i = 0; i < CpuUtilities::GetProcessorCount(); i++)
            {
                gdt[i].Initialise();
            }
        }
        
        gdt[CpuUtilities::GetCurrentProcessorId()].Load();
    }
}
