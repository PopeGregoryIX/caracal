/*
 * lfbconsoleoutput.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */
#include <stdint.h>
#include <debug/lfbconsoleoutput.h>
#include <bootboot.h>

extern uint8_t fb;                      // linear framebuffer mapped

namespace arch
{
	LfbConsoleOutput LfbConsoleOutput::_instance;

	void LfbConsoleOutput::PutChar(char)
	{

	}

	void LfbConsoleOutput::PutString(const char* s)
	{
		uint32_t x,y,kx=0,line,mask,offs;

		int bpl=(font->width+7)/8;

			while(*s) {
				unsigned char *glyph = GetGlyph(*s);
				offs = (kx * (font->width+1) * 4);
				for(y=0;y<font->height;y++) {
					line=offs; mask=1<<(font->width-1);
					for(x=0;x<font->width;x++) {
						*((uint32_t*)((uint64_t)&fb+line))=((int)*glyph) & (mask)?0xFFFFFF:0;
						mask>>=1; line+=4;
					}
					*((uint32_t*)((uint64_t)&fb+line))=0; glyph+=bpl; offs+=bootboot.fb_scanline;
				}
				s++; kx++;
			}
	}
}
