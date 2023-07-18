#ifndef __CBOOT_H_
#define __CBOOT_H_

#include <stdint.h>

#define CBOOT_MAGIC 0x0CA8ACA1

#define CBOOT_VERSION   1

struct CBoot_PC
{
    uint64_t gdtAddress;
    uint64_t gdtBytes;
} __attribute__((packed));

struct CBoot_ARM
{
    uint64_t mmioAddress;
    uint64_t mmioBytes;
} __attribute__((packed));

union CBoot_Arch
{
    CBoot_PC*  cbootArchPC;
    CBoot_ARM* cbootArchArm;
};

struct CBoot
{
    uint64_t magic;             // 0x0CA8ACAl
    uint32_t version;           // 1
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
    CBoot_Arch cbootArchData;
} __attribute__((packed));

extern CBoot cboot;

#endif
