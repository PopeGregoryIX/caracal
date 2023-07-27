#ifndef __TAR__H__
#define __TAR__H__

#include <stdint.h>
#include <stddef.h>
#include <cxx.h>

#define TMAGIC   "ustar"        /* ustar and a null */
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2

/* Values used in typeflag field.  */
#define REGTYPE  '0'
#define AREGTYPE '\0'
#define LNKTYPE  '1'
#define SYMTYPE  '2'
#define CHRTYPE  '3'
#define BLKTYPE  '4'
#define DIRTYPE  '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'


class Tar
{
    public:
        Tar(void* tarData){ _currentDirectory = _data = (TarHeader*)tarData; }

        struct TarHeader
        {
            unsigned char fileName[100];
            uint64_t fileMode;
            uint64_t ownerId;
            uint64_t groupId;
            unsigned char fileSize[12];
            unsigned char modifiedTime[12];
            uint64_t checkSum;
            char type;
            unsigned char linkedFileName[100];
            unsigned char ustar[6];
            uint16_t ustarVersion;
            unsigned char userName[32];
            unsigned char groupName[32];
            uint64_t deviceMajorNumber;
            uint64_t deviceMinorNumber;
            unsigned char fileNamePrefix[155];
        } __attribute__((packed));

        inline size_t GetCurrentDirectorySize( void ) { return octToBin(_currentDirectory->fileSize, 11); }

        void PrintDirectoryListing( TarHeader* cd = nullptr );

        uintptr_t GetEntryHandle(const char* path);
        uintptr_t GetFileSize(uintptr_t handle);
        void* GetFileData(uintptr_t handle);
        TarHeader* GetNextHeader(TarHeader* header);
    private:
        TarHeader* _data;
        TarHeader* _currentDirectory;
};

#endif