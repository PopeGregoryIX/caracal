#ifndef __CBOOT_H_
#define __CBOOT_H_

#include <stdint.h>

struct CBoot
{
    char magic[8];
    uint32_t version;
    uint32_t size;
    uint32_t bspId;
    uint32_t cpuCount;
    uint64_t lfbAddress;
    uint64_t lfbSize;
    uint32_t lfbFormat;
    uint32_t lfbScreenWidth;
    uint32_t lfbScreenHeight;
    uint32_t lfbScanlineBytes;
    uint64_t mmapAddress;
    uint64_t mmapBytes;
    uint64_t configStringAddress;
    uint64_t configStringBytes;
    uint64_t mmioAddress;
    uint64_t mmioBytes;
    uint64_t gdtAddress;
    uint64_t gdtBytes;
} __attribute__((packed));


#endif
