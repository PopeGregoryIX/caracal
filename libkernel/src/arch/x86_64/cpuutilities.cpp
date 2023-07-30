#include <arch/cpuutilities.h>
#include <c_cpuid.h>
#include <stdint.h>
#include <stddef.h>
#include <paging.h>
#include <caracal.h>
#include <debug.h>

int CpuUtilities::_bspId;
uintptr_t (*CpuUtilities::_getSmallPagingStructure)( void );
uintptr_t (*CpuUtilities::_getLargePagingStructure)( void );

void CpuUtilities::EnsureMemoryAccessible(uintptr_t address, size_t bytes, uint32_t flags)
{
    size_t pageSize = ((flags & CARACAL_MEMORY_KERNEL_CODE) == CARACAL_MEMORY_KERNEL_CODE) ? 0x200000 : 0x1000;
    size_t cpuFlags;
    
    if((bytes % pageSize) != 0) bytes+= (pageSize - (bytes % pageSize));
    if((address % pageSize) != 0) address-= (address % pageSize);

    cpuFlags = PAGE_PRESENT | PAGE_WRITE;
    if((flags & CARACAL_MEMORY_KERNEL_CODE) == CARACAL_MEMORY_KERNEL_CODE) cpuFlags |= PAGE_GLOBAL;
    
    for(uintptr_t i = address; i < address + bytes; i += pageSize)
    {
        if(!arch::Paging::IsPagedIn(address))
        {
            if(pageSize == 0x200000)
            {
                arch::Paging::PageIn2m(cpuFlags, address, _getLargePagingStructure());
            }
            else
            {
                arch::Paging::PageIn4k(cpuFlags, address, _getSmallPagingStructure());
            }
        }
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