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

namespace arch
{
	class LfbConsoleOutput : public ConsoleOutput
	{
	public:
		static LfbConsoleOutput& GetInstance( void ) { return _instance; }

		void PutChar(const char c);
		void PutString(const char* s);
	private:
		static LfbConsoleOutput _instance;

		const psf2_t *font = (const psf2_t*)&_binary_src_data_font_psf_start;

		inline uint8_t* GetGlyph(const char c)
		{
			return (uint8_t*)&_binary_src_data_font_psf_start + font->headersize +
									(c>0&&(uint32_t)(c)<font->numglyph?c:0)*font->bytesperglyph;
		}
	};
}

#endif /* KERNEL_INCLUDE_ARCH_MACHINE_PC_DEBUG_LFBCONSOLEOUTPUT_H_ */
