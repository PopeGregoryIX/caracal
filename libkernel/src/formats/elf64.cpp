#include <format/elf64.h>
#include <cxx.h>
#include <stdint.h>
#include <stddef.h>
#include <debug.h>

uintptr_t Elf64::GetLowestAddress( void )
{
    uintptr_t lowest = 0xFFFFFFFFFFFFFFFF;
    for(size_t i = 0; i < _header->e_phnum; i++)
    {
        uintptr_t current = GetProgramHeader(i).p_vaddr;
        if(current < lowest) lowest = current;
    }

    return lowest;
}

uintptr_t Elf64::GetMemorySize( void )
{
    uintptr_t size = 0;
    for(size_t i = 0; i < _header->e_phnum; i++)
    {
        size+= GetProgramHeader(i).p_memsz;
    }

    return size;
}

void Elf64::Relocate( void )
{
    if(_header->e_phnum == 0)
        FATAL("No ELf program headers found.");

    for(size_t i = 0; i < _header->e_phnum; i++)
    {
        ProgramHeader& programHeader = GetProgramHeader(i);

        if(programHeader.p_memsz > 0)
        {
            memcpy((void*)programHeader.p_vaddr, (void*)GetFileOffset(programHeader.p_offset), programHeader.p_filesz);

            if(programHeader.p_memsz > programHeader.p_filesz)
            {
                memset((void*)(programHeader.p_vaddr + programHeader.p_filesz), 0, programHeader.p_memsz - programHeader.p_filesz);
            }
        }
    }
}

void Elf64::Start( void )
{
    void (*start)() = (void (*)())(_header->e_entry);
    start();
}