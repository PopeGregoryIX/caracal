/*
 * lfbconsoleoutput.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */
#include <stdint.h>
#include <debug/lfbconsoleoutput.h>
#include <bootboot.h>
#include <debug/consolecolour.h>
#include <runtime/cxx.h>
#include <support/string.h>

namespace arch
{
	LfbConsoleOutput LfbConsoleOutput::_instance;

	LfbConsoleOutput::LfbConsoleOutput( void )
	{
		_widthChars = bootboot.fb_scanline / font->width;
		_heightChars = bootboot.fb_height / font->height;
		_currentX = _currentY = 0;
		_bytesPerPixel = 4;
		_tabStop = 5;
	}

	void LfbConsoleOutput::Cls( void )
	{
		uint32_t* lfb = (uint32_t*)&fb;

		for(size_t i = 0; i <  bootboot.fb_width * bootboot.fb_height; ++i)
			lfb[i] = _colour.Background;

		_currentX = _currentY = 0;
	}

	void LfbConsoleOutput::PutChar(const char c)
	{
		int glyphBytesPerLine = (font->width + 7) / 8;
		unsigned char *glyph = GetGlyph(c);

		switch(c)
		{
			case('\n'):
				_currentX = 0;
				_currentY++;
				break;
			case('\t'):
				_currentX+= 5 - (_currentX % _tabStop);
				break;
			default:
				int offset =	(_currentY * font->height * bootboot.fb_scanline) +
								(_currentX * (font->width + 1) * _bytesPerPixel);

				unsigned int glyphX, glyphY, glyphLine, glyphMask;

				for(glyphY=0;glyphY<font->height;glyphY++){
					/* save the starting position of the line */
					glyphLine=offset;
					glyphMask=1<<(font->width-1);
					/* display a row */
					for(glyphX=0;glyphX<=font->width;glyphX++){
						*((uint32_t*)((uint64_t)&fb+glyphLine)) =
								*((unsigned int*)glyph) & glyphMask ? _colour.Foreground : _colour.Background;
						/* adjust to the next pixel */
						glyphMask >>= 1;
						glyphLine += _bytesPerPixel;
					}
					/* adjust to the next line */
					glyph += glyphBytesPerLine;
					offset  += bootboot.fb_scanline;
				}
				_currentX++;
		}

		if(_currentX >= _widthChars)
		{
			_currentX = 0;
			_currentY++;
		}

		if(_currentY > _heightChars)
		{
			Scroll();
			_currentX = 0;
			_currentY = _heightChars;
		}

	}

	void LfbConsoleOutput::Scroll( void )
	{
		uint32_t* dest = (uint32_t*)&fb;
		uint32_t* src = &dest[bootboot.fb_width * font->height];

		::memorycopy<uint32_t>(dest, src, bootboot.fb_width * ((bootboot.fb_height * font->height) -1));

		uint32_t startClear = bootboot.fb_width * ((bootboot.fb_height * font->height) -1);
		uint32_t endClear = bootboot.fb_width * ((bootboot.fb_height * font->height));

		for(uint32_t c = startClear; c < endClear ; ++c)
			dest[c] = _colour.Background;
	}
}
