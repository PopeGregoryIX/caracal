#include <debug.h>
#include <stdint.h>
#include <stddef.h>
#include <cxx.h>
#include <format/tar.h>

void Tar::PrintDirectoryListing( TarHeader* cd )
{
    if(cd == nullptr) cd = _currentDirectory;
    if(strncmp((char*)cd->ustar, (char*)"ustar", 5) != 0) FATAL("Tar header is invalid.");
    while(strncmp((char*)cd->ustar, (char*)"ustar", 5) == 0)
    {
        if(cd->type == LNKTYPE) INFO("LINK");
        INFO((char*)(cd->fileName) << "(" << (uintptr_t)GetFileSize((uintptr_t)cd) << ")");
        cd = GetNextHeader(cd);
    }
}

uintptr_t Tar::GetEntryHandle(const char* path)
{
    int pathLength = (int)strlen(path);

    TarHeader* cd = _currentDirectory;

    while(strncmp((char*)cd->ustar, (char*)"ustar", 5) == 0)
    {
        if(strncmp((char*)(cd->fileName), path, pathLength) == 0) return (uintptr_t)cd;
        cd = GetNextHeader(cd);
    }

    return 0;
}

void* Tar::GetFileData(uintptr_t handle)
{
    return (void*)(handle + 512);
}

uintptr_t Tar::GetFileSize(uintptr_t handle)
{
    TarHeader* cd = (TarHeader*)handle;
    return octToBin(cd->fileSize, 11);
}

Tar::TarHeader* Tar::GetNextHeader(TarHeader* header)
{
    uintptr_t size = GetFileSize((uintptr_t)header);

    if(size == 0)
        return (TarHeader*)(((uintptr_t)header) + 512);
    else
    {
        if(size % 512 != 0) size = size + (512 - (size % 512));
        return (TarHeader*)(((uintptr_t)header) + 512 + size);
    }
}