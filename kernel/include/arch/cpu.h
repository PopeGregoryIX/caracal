#ifndef __ARCH__CPH__H
#define __ARCH__CPH__H

namespace arch
{
    class Cpu
    {
        public:
            /**
             * @brief Ensure that PageFrameAllocator is able to access the memory it needs.
             * 
             * Use CpuUtilities to provide PFA with allocator routines.
             */
            static void EarlyMemorySetup( CBoot& cboot );
    };
}

#endif
