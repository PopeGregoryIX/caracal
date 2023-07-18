#ifndef __BMAIN_H_
#define __BMAIN_H_

int tar_lookup(unsigned char *archive, char *filename, char **out);

void PageInLarge(uintptr_t flags, uintptr_t virtualAddress, uintptr_t physicalAddress);
uint64_t ReadCr3(void);
void WriteCr3(uint64_t value);
pageDirectoryEntry_t* GetNewPagingStructure( void );
bool IsPagedIn(uintptr_t virtualAddress);

#endif
