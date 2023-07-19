#include <debug/debug.h>
#include <stdint.h>
#include <stddef.h>
#include <support/string.h>
#include <tar.h>

void Tar::PrintDirectoryListing( void )
{
    TarHeader* cd = _currentDirectory;

    while(String::compare((char*)cd->ustar, (char*)"ustar", 5) == 0)
    {
        cd = (TarHeader*)(((uintptr_t)cd) + 512);
    }
}

uintptr_t Tar::GetEntryHandle(const char* path)
{
    int pathLength = (int)String::length(path);

    TarHeader* cd = _currentDirectory;

    while(String::compare((char*)cd->ustar, (char*)"ustar", 5) == 0)
    {
        if(String::compare((char*)(cd->fileName), path, pathLength) == 0) return (uintptr_t)cd;
        cd = (TarHeader*)(((uintptr_t)cd) + 512);
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
    return String::octToBin(cd->fileSize, 11);
}