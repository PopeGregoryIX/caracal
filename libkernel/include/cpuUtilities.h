#ifndef __CPU__UTILITIES
#define __CPU__UTILITIES

class CpuUtilities
{
    public:
        static inline int GetBspId( void ) { return _bspId; }
        static inline void SetBspId( int bspId ) { _bspId = bspId; }

        static int GetCurrentProcessorId( void );
        static int GetProcessorCount( void );

        static inline bool IsBsp( void ) { return GetCurrentProcessorId() == GetBspId(); }
    private:
        static int _bspId;
};

#endif