#include <c_cpuid.h>
#include <cpuid.h>
#include <stdint.h>
#include <stddef.h>

namespace arch
{
    CpuId_Features CpuId::GetFeatures( void )
    {
        CpuId_Generic generic = GetResult(CPUID_FEATURES);

        return CpuId_Features(generic);
    }

    CpuId_Generic CpuId::GetResult(uint32_t cpuidFunction)
    {
        CpuId_Generic result;
        __cpuid(cpuidFunction, result.eax, result.ebx, result.ecx, result.edx);
        return result;
    }
}