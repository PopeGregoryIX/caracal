#include <cpuutilities.h>
#include <c_cpuid.h>
#include <stdint.h>
#include <stddef.h>
#include <paging.h>
#include <caracal.h>

int CpuUtilities::_bspId;

void CpuUtilities::EnsureMemoryAccessible(uintptr_t address, size_t bytes, uint32_t flags)
{
    size_t pageSize = ((flags & CARACAL_MEMORY_KERNEL_CODE) == CARACAL_MEMORY_KERNEL_CODE) ? 0x200000 : 0x1000;
    
    if((bytes % pageSize) != 0) bytes+= (pageSize - (bytes % pageSize));
    if((address % pageSize) != 0) address-= (address % pageSize);

    if(!arch::Paging::IsPagedIn(address))
    {
        
    }
}

int CpuUtilities::GetCurrentProcessorId( void )
{
    return arch::CpuId::GetFeatures().brand.brandFeatures.apicId;
}

int CpuUtilities::GetProcessorCount( void )
{
    return arch::CpuId::GetFeatures().brand.brandFeatures.cpuCount;
}