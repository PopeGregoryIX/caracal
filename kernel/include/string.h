#ifndef __KSTRING_H
#define __KSTRING_H

#include <stdint.h>
#include <cpu.h>

class String
{
public:

private:

public:
    static int compare(const char* s1, const char* s2, int maxLength = 0);
	static char* copy(char* dest, const char* src);
	static char* itoa(uint64_t num, char* buffer, uint8_t base = 16);
	static size_t length(const char* s);

private:
	static uint8_t numBase_;
	static const char ltHexUpper_[16];
	static const char ltHexLower_[16];
};

#endif
