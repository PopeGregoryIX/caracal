#include <cxx.h>
#include <stdint.h>

const char ltHexUpper_[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const char ltHexLower_[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

char* itoa(uint64_t num, char* buffer, uint8_t base) -> string
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