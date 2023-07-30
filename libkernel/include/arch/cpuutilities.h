#ifndef __CPU__UTILITIES
#define __CPU__UTILITIES

#include <stdint.h>
#include <stddef.h>

class CpuUtilities
{
    public:
        inline static void SetPageRoutines(uintptr_t (*getSmallPagingStructure)( void ), uintptr_t (*getLargePagingStructure)( void ))
        {
            _getLargePagingStructure = getLargePagingStructure;
            _getSmallPagingStructure = getSmallPagingStructure;
        }
        
        static void EnsureMemoryAccessible(uintptr_t address, size_t bytes, uint32_t flags);

        static inline int GetBspId( void ) { return _bspId; }
        static inline void SetBspId( int bspId ) { _bspId = bspId; }

        static int GetCurrentProcessorId( void );
        static int GetProcessorCount( void );

        static inline bool IsBsp( void ) { return GetCurrentProcessorId() == GetBspId(); }
    private:
        static int _bspId;
        static uintptr_t (*_getSmallPagingStructure)( void );
        static uintptr_t (*_getLargePagingStructure)( void );
};

#endif