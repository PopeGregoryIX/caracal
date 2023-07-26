#include <stdint.h>
#include <stddef.h>
#include <debug/lfbconsoleoutput.h>
#include <cxx.h>

LfbConsoleOutput::LfbConsoleOutput(uintptr_t fb, const psf2_t* font, uint32_t width, uint32_t height, uint32_t scanlineBytes, uint32_t bytesPerPixel )
: 	_widthBytes(width), _heightBytes(height), _currentX(0), _currentY(0), _tabStop(5),
	_bytesPerPixel(bytesPerPixel), _scanlineBytes(scanlineBytes), _fb(fb), _font(font)
{
    _widthChars = _heightBytes / _font->width;
    _heightChars = _widthBytes / _font->height;
}

void LfbConsoleOutput::Cls( void )
{
	uint32_t* lfb = (uint32_t*)_fb;

	for(size_t i = 0; i <  _widthBytes * _heightBytes; ++i) lfb[i] = _colour.Background;
	_currentX = _currentY = 0;
}

void LfbConsoleOutput::PutChar(const char c)
{
	int glyphBytesPerLine = (_font->width + 7) / 8;
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
			int offset =	(_currentY * _font->height * _scanlineBytes) +
							(_currentX * (_font->width + 1) * _bytesPerPixel);

			unsigned int glyphX, glyphY, glyphLine, glyphMask;

			for(glyphY=0;glyphY<_font->height;glyphY++){
				/* save the starting position of the line */
				glyphLine=offset;
				glyphMask=1<<(_font->width-1);
				/* display a row */
				for(glyphX=0;glyphX<=_font->width;glyphX++){
					*((uint32_t*)((uint64_t)_fb+glyphLine)) =
							*((unsigned int*)glyph) & glyphMask ? _colour.Foreground : _colour.Background;
					/* adjust to the next pixel */
					glyphMask >>= 1;
					glyphLine += _bytesPerPixel;
				}
				/* adjust to the next line */
				glyph += glyphBytesPerLine;
				offset  += _scanlineBytes;
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
	
}