#include <cpuUtilities.h>
#include <c_cpuid.h>
#include <stdint.h>
#include <stddef.h>

int CpuUtilities::_bspId;

int CpuUtilities::GetCurrentProcessorId( void )
{
    return arch::CpuId::GetFeatures().brand.brandFeatures.apicId;
}

int CpuUtilities::GetProcessorCount( void )
{
    return arch::CpuId::GetFeatures().brand.brandFeatures.cpuCount;
}