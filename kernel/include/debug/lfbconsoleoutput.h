/*
 * lfbconsoleoutput.h
 *
 *  Created on: 20 Sept 2021
 *      Author: Adam
 */

#ifndef KERNEL_INCLUDE_ARCH_MACHINE_PC_DEBUG_LFBCONSOLEOUTPUT_H_
#define KERNEL_INCLUDE_ARCH_MACHINE_PC_DEBUG_LFBCONSOLEOUTPUT_H_

#include <stdint.h>
#include <debug/consoleoutput.h>
#include <pcscreenfont.h>
#include <debug/consolecolour.h>

namespace arch
{
	extern "C" uint8_t fb;		//	bootboot linear framebuffer

	class LfbConsoleOutput : public ConsoleOutput
	{
	public:
		static LfbConsoleOutput& GetInstance( void ) { return _instance; }
		LfbConsoleOutput( void );

		void PutChar(const char c);

		void Cls ( void );

		void Scroll( void );
	private:
		static LfbConsoleOutput _instance;

		uint32_t _widthChars, _heightChars;
		uint32_t _currentX, _currentY, _tabStop;
		uint32_t _bytesPerPixel;

		const psf2_t *font = (const psf2_t*)&_binary_src_data_font_psf_start;

		inline uint8_t* GetGlyph(const char c)
		{
			return (uint8_t*)&_binary_src_data_font_psf_start + font->headersize +
									(c>0&&(uint32_t)(c)<font->numglyph?c:0)*font->bytesperglyph;
		}
	};
}

#endif /* KERNEL_INCLUDE_ARCH_MACHINE_PC_DEBUG_LFBCONSOLEOUTPUT_H_ */
