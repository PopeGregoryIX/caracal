#ifndef __ELF64__H__
#define __ELF64__H__

#include <stdint.h>
#include <stddef.h>
#include <cxx.h>
#include <debug.h>

typedef uint64_t  Elf64_Addr;
typedef uint16_t  Elf64_Half;
typedef uint64_t  Elf64_Off;
typedef uint32_t   Elf64_Sword;
typedef int64_t   Elf64_Sxword;
typedef int32_t  Elf64_Word;
typedef uint64_t  Elf64_Lword;
typedef uint64_t  Elf64_Xword;

#define EI_NIDENT           16

#define ELF_BITS_32         1
#define ELF_BITS_64         2

#define ELF_LITTLE_ENDIAN   1
#define ELF_BIG_ENDIAN      2

#define ELF_ARCH_X86_32     0x03
#define ELF_ARCH_ARM32      0x28        
#define ELF_ARCH_X86_64     0x3E
#define ELF_ARCH_ARM64      0xB7

class Elf64
{
    public:
        Elf64(void* imgStart) { _header = (Header*)imgStart; }

        inline void* GetImageStart( void ) { return _header; }

        inline bool IsValid( void ) 
        { 
            return ((_header->e_ident[0] == 0x7F) && (_header->e_ident[1] == 'E')
                    && (_header->e_ident[2] == 'L') && (_header->e_ident[3] == 'F'));
        }

        inline bool IsValid( unsigned char desiredBitCode, unsigned char desiredEndianness)
        {
            return (IsValid() && (_header->e_ident[4] == desiredBitCode) && (_header->e_ident[5] == desiredEndianness));
        }

        inline Elf64_Half GetArchitecture( void ) { return _header->e_machine; }

        uintptr_t GetLowestAddress( void );

        uintptr_t GetMemorySize( void );

        uintptr_t GetSymbolAddress( const char* symName ) { return GetSymbol(symName).st_value; }

        inline uintptr_t GetEntry( void ) { return _header->e_entry; }

        void Relocate( void );

        void Start( void );

        struct Header
        {
            unsigned char   e_ident[EI_NIDENT]; /* File identification. */
            Elf64_Half      e_type;             /* File type. */
            Elf64_Half      e_machine;          /* Machine architecture. */
            Elf64_Word      e_version;          /* ELF format version. */
            Elf64_Addr      e_entry;            /* Entry point. */
            Elf64_Off       e_phoff;            /* Program header file offset. */
            Elf64_Off       e_shoff;            /* Section header file offset. */
            Elf64_Word      e_flags;            /* Architecture-specific flags. */
            Elf64_Half      e_ehsize;           /* Size of ELF header in bytes. */
            Elf64_Half      e_phentsize;        /* Size of program header entry. */
            Elf64_Half      e_phnum;            /* Number of program header entries. */
            Elf64_Half      e_shentsize;        /* Size of section header entry. */
            Elf64_Half      e_shnum;            /* Number of section header entries. */
            Elf64_Half      e_shstrndx;         /* Section name strings section. */
        } __attribute__((packed));
        
        struct ProgramHeader
        {
            Elf64_Word      p_type;         /* Entry type. */
            Elf64_Word      p_flags;        /* Access permission flags. */
            Elf64_Off       p_offset;       /* File offset of contents. */
            Elf64_Addr      p_vaddr;        /* Virtual address in memory image. */
            Elf64_Addr      p_paddr;        /* Physical address (not used). */
            Elf64_Xword     p_filesz;       /* Size of contents in file. */
            Elf64_Xword     p_memsz;        /* Size of contents in memory. */
            Elf64_Xword     p_align;        /* Alignment in memory and file. */
        };

        struct SectionHeader
        {
            Elf64_Word      sh_name;        /* Section name (index into the section header string table). */
            Elf64_Word      sh_type;        /* Section type. */
            Elf64_Xword     sh_flags;       /* Section flags. */
            Elf64_Addr      sh_addr;        /* Address in memory image. */
            Elf64_Off       sh_offset;      /* Offset in file. */
            Elf64_Xword     sh_size;        /* Size in bytes. */
            Elf64_Word      sh_link;        /* Index of a related section. */
            Elf64_Word      sh_info;        /* Depends on section type. */
            Elf64_Xword     sh_addralign;   /* Alignment in bytes. */
            Elf64_Xword     sh_entsize;     /* Size of each entry in section. */
        };

        struct Symbol
        {
            Elf64_Word      st_name;        /* String table index of name. */
            unsigned char   st_info;        /* Type and binding information. */
            unsigned char   st_other;       /* Reserved (not used). */
            Elf64_Half      st_shndx;       /* Section index of symbol. */
            Elf64_Addr      st_value;       /* Symbol value. */
            Elf64_Xword     st_size;        /* Size of associated object. */
        };

    private:
        Header* _header = nullptr;
        unsigned char* _sectionNameTable = nullptr;
        unsigned char* _stringTable = nullptr;
        Symbol* _symbolTable = nullptr;
        size_t _symbolTableEntries = 0;

        inline void* GetFileOffset(size_t offset) { return (void*)(((uintptr_t)_header) + offset);  }

        inline ProgramHeader& GetProgramHeader(size_t index) { return *((ProgramHeader*)GetFileOffset(_header->e_phoff + (index * _header->e_phentsize))); }
        
        inline SectionHeader& GetSectionHeader(size_t index) { return *((SectionHeader*)GetFileOffset(_header->e_shoff + (index * _header->e_shentsize))); }

        inline SectionHeader& GetSectionHeader(const char* name)
        {
            for(int i = 0; i < _header->e_shnum; i++)
            {
                SectionHeader& sectionHeader = GetSectionHeader(i);
                unsigned char* sectionName = &(GetSectionNameTable()[sectionHeader.sh_name]);
                if(strlen((const char*)sectionName) == 0) continue;
                if(strcmp(name, (const char*)sectionName) == 0) return sectionHeader;
            }

            FATAL("Search for non-existent ELF header " << name);
            return *((SectionHeader*)0);
        }

        inline unsigned char* GetSectionNameTable( void ) 
        { 
            return (_sectionNameTable = (_sectionNameTable == nullptr) ? 
                    (unsigned char*)(GetFileOffset(GetSectionHeader(_header->e_shstrndx).sh_offset)) :
                    _sectionNameTable); 
        }

        inline unsigned char* GetStringTable( void )
        {
            return (_stringTable = (_stringTable == nullptr) ? 
                    (unsigned char*)(GetFileOffset(GetSectionHeader(".strtab").sh_offset)) :
                    _stringTable); 
        }

        inline Symbol* GetSymbolTable( void )
        {
            return (_symbolTable = (_symbolTable == nullptr) ? 
                (Symbol*)(GetFileOffset(GetSectionHeader(".symtab").sh_offset)) :
                _symbolTable); 
        }

        size_t GetSymbolTableEntryCount( void )
        {
            return (_symbolTableEntries = (_symbolTableEntries == 0) ?
                GetSectionHeader(".symtab").sh_size / sizeof(Symbol) :
                _symbolTableEntries);
        }

        inline Symbol& GetSymbol( const char* name )
        {
            INFO(GetSectionHeader(".symtab").sh_size);
            for(size_t i = 0; i < GetSymbolTableEntryCount(); i++)
            {
                Symbol* symbol = &(GetSymbolTable()[i]);
                const char* symbolName = (const char*)(&(GetStringTable()[symbol->st_name]));
                if(strcmp(name, symbolName) == 0) return *symbol;
            }

            FATAL("Search for non-existent Symbol " << name);
            return *((Symbol*)0);
        }
};

#endif