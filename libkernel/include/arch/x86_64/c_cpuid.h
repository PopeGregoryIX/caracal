#ifndef __CPUID___H_
#define __CPUID___H_

#include <stdint.h>
#include <stddef.h>

namespace arch
{
    #define CPUID_VENDOR    0x00
    #define CPUID_FEATURES  0x01

    struct CpuId_Generic
    {
        uint32_t eax, ebx, ecx, edx;
    };


    struct Features_BrandId
    {
        uint32_t brand:8;
        uint32_t clFlushChunkCount:8;
        uint32_t cpuCount:8;
        uint32_t apicId:8;
    } __attribute__((packed));

    struct CpuId_Features
    {
        uint32_t eax;
        union
        {
            uint32_t ebx;
            Features_BrandId brandFeatures;
        } brand;
        uint32_t ecx;
        uint32_t edx;

        CpuId_Features(CpuId_Generic generic) : 
            eax(generic.eax),
            ecx(generic.ecx),
            edx(generic.edx){ brand.ebx = generic.ebx; }
    };

    class CpuId
    {
        public:
            static CpuId_Features GetFeatures( void );

        private:
            static CpuId_Generic GetResult(uint32_t cpuidFunction);
    };
}

#endif
