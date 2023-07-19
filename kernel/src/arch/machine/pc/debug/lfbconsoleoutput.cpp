/*
 * lfbconsoleoutput.cpp
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */
#include <stdint.h>
#include <debug/lfbconsoleoutput.h>
#include <cboot.h>
#include <debug/consolecolour.h>
#include <cxx.h>
#include <support/string.h>

namespace arch
{
	LfbConsoleOutput LfbConsoleOutput::_instance;

	LfbConsoleOutput::LfbConsoleOutput( void )
	{
		_widthChars = cboot.lfbScanlineBytes / font->width;
		_heightChars = cboot.lfbScreenHeight / font->height;
		_currentX = _currentY = 0;
		_bytesPerPixel = 4;
		_tabStop = 5;
	}

	void LfbConsoleOutput::Cls( void )
	{
		uint32_t* lfb = (uint32_t*)cboot.lfbAddress;

		for(size_t i = 0; i <  cboot.lfbScreenWidth * cboot.lfbScreenHeight; ++i)
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
				int offset =	(_currentY * font->height * cboot.lfbScanlineBytes) +
								(_currentX * (font->width + 1) * _bytesPerPixel);

				unsigned int glyphX, glyphY, glyphLine, glyphMask;

				for(glyphY=0;glyphY<font->height;glyphY++){
					/* save the starting position of the line */
					glyphLine=offset;
					glyphMask=1<<(font->width-1);
					/* display a row */
					for(glyphX=0;glyphX<=font->width;glyphX++){
						*((uint32_t*)((uint64_t)cboot.lfbAddress+glyphLine)) =
								*((unsigned int*)glyph) & glyphMask ? _colour.Foreground : _colour.Background;
						/* adjust to the next pixel */
						glyphMask >>= 1;
						glyphLine += _bytesPerPixel;
					}
					/* adjust to the next line */
					glyph += glyphBytesPerLine;
					offset  += cboot.lfbScanlineBytes;
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
		uint32_t* dest = (uint32_t*)cboot.lfbAddress;
		uint32_t* src = &dest[cboot.lfbScreenWidth * font->height];

		::memorycopy<uint32_t>(dest, src, cboot.lfbScreenWidth * ((cboot.lfbScreenHeight * font->height) -1));

		uint32_t startClear = cboot.lfbScreenWidth * ((cboot.lfbScreenHeight * font->height) -1);
		uint32_t endClear = cboot.lfbScreenWidth * ((cboot.lfbScreenHeight * font->height));

		for(uint32_t c = startClear; c < endClear ; ++c)
			dest[c] = _colour.Background;
	}
}
