/*
 * String.cpp
 *
 *  Created on: 12 Jun 2013
 *      Author: Adam
 */
#include <cxx.h>
#include <support/string.h>

uint8_t String::numBase_ = 16;
const char String::ltHexUpper_[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const char String::ltHexLower_[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

int String::compare(const char* s1, const char* s2, int maxLength)
{
    int len = (int)length(s1);
	int len2 = (int)length(s2);

	if((maxLength == 0) && (len != len2)) return 1;
    if((maxLength != 0) && (len > maxLength)) len = maxLength;

    for(int i = 0; i < maxLength; ++i)
    {
        if(s1[i] != s2[i])
            return s1[i] - s2[i];
    }

    return 0;
}

char* String::copy(char* dest, const char* src)
{
	int len = (int)(length(src) + 1);
	if(dest > src)
		while(--len >=0) dest[len] = src[len];
	else
		for(int i=0; i < len; ++i) dest[i] = src[i];

	return dest;
}

size_t String::length(const char* s)
{
	char* str = (char*)s;
	size_t i = 0;
	while(*str++) i++;
	return i;
}

char* String::itoa(uint64_t num, char* buffer, uint8_t base)
{
	int index = 0;
	int bitshift;

	switch(base)
	{
	case 2:
		// Binary
		bitshift = 64;
		while(bitshift != 0)
		{
			bitshift -= 1;
			int digit = (num >> bitshift) & 0x01;
			buffer[index] = ltHexUpper_[digit];
			index++;
		}
		break;
	case 10:
	{
		//	Create a temporary buffer, then reverse
		char newBuffer[65];

		while(num != 0)
		{
			int digit = (int)(num % 10);
			newBuffer[index] = ltHexUpper_[digit];
			index++;
			num/=10;
		}

		int i = 0;
		while(index--) buffer[i++] = newBuffer[index];
		index = i;
		break;
	}
	case 16:
		//	hexadecimal
		bitshift = 64;
		while(bitshift != 0)
		{
			bitshift -= 4;
			int digit = (num >> bitshift) & 0x0F;
			buffer[index] = ltHexUpper_[digit];
			if(index != 0 || digit != 0) index++;
		}
		break;
	default:
		break;
	}

	if(index == 0) buffer[index++] = '0';
	buffer[index] = 0;

	return buffer;
}

size_t String::octToBin(unsigned char *str, int size) 
{
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

