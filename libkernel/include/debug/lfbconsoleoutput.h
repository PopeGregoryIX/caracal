#ifndef __LFB__TEXT__CONSOLE__
#define __LFB__TEXT__CONSOLE__

#include <stdint.h>
#include <stddef.h>
#include <debug/consoleoutput.h>
#include <bootboot.h>
#include <format/pcscreenfont.h>

class LfbConsoleOutput : public ConsoleOutput
{
    public:
		LfbConsoleOutput( void );

		LfbConsoleOutput(uintptr_t fb, const psf2_t* font, uint32_t width, uint32_t height, uint32_t scanlineBytes, uint32_t bpp );

		void Initialise(uintptr_t fb, const psf2_t* font, uint32_t width, uint32_t height, uint32_t scanlineBytes, uint32_t bpp );

		void PutChar(const char c);

		void Cls ( void );

		void Scroll( void );

        virtual ~LfbConsoleOutput(){}
    private:
        uint32_t _widthBytes, _heightBytes, _widthChars, _heightChars, _currentX, _currentY, _tabStop;
		uint32_t _bytesPerPixel, _scanlineBytes;
		uintptr_t _fb;
		const psf2_t *_font;

		inline uint8_t* GetGlyph(const char c)
		{
			return (uint8_t*)&_binary_src_data_font_psf_start + _font->headersize +
									(c>0&&(uint32_t)(c)<_font->numglyph?c:0)*_font->bytesperglyph;
		}
};

#endif
