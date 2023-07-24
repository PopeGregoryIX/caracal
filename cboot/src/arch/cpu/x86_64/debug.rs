#[cfg(target_arch = "x86_64")]

extern crate rlibc;

pub const LFB: u64 = 0xfffffffffc000000;

#[doc = " Display text on screen *"]
#[repr(C, packed)]
#[derive(Debug, Copy, Clone)]
pub struct psf2_t {
    pub magic: u32,
    pub version: u32,
    pub headersize: u32,
    pub flags: u32,
    pub numglyph: u32,
    pub bytesperglyph: u32,
    pub height: u32,
    pub width: u32,
    pub glyphs: u8,
}

extern "C" {
    pub static mut _binary___src_data_font_psf_start: u64;
}

#[derive(Debug, Copy, Clone)]
pub struct DebugOutput
{
    pub heightChars: u16,
    pub widthChars: u16,
    pub currentX: u32,
    pub currentY: u32,
    pub tabStop: u32,
    pub bytesPerScanline: u32,
    pub bytesPerPixel: u32,
    pub lfb: u64,
    pub font: *mut psf2_t
}

impl DebugOutput
{
    pub fn Puts(&mut self, string: &'static str)
    {
        for s in string.bytes() {   self.Putc(s); }
    }

    pub fn Putc(&mut self, c: u8)
    {
    
        if c == '\n' as u8
        {
            self.currentX = 0;
            self.currentY += 1;
        }
        else if c == '\t' as u8
        {
            self.currentX += self.tabStop;
        }
        else
        {
            unsafe
            {
                let mut glyph = self.GetGlyph(c);

                // (_currentY * font->height * cboot.lfbScanlineBytes) +
				//(_currentX * (font->width + 1) * _bytesPerPixel);
                let mut offset = (self.currentY * ((*(self.font)).height) * self.bytesPerScanline) +
                                 (self.currentX * ((*(self.font)).width + 1) * self.bytesPerPixel);
                for _glyphY in 0..(*(self.font)).height
                {
                    let mut line = offset as u64;
                    let mut mask = 1 << ((*(self.font)).width - 1);

                    for _glyphX in 0..(*(self.font)).width 
                    {
                        let target_location = (self.lfb as *const u8 as u64 + line) as *mut u32;
                        let mut target_value: u32 = 0;
                        if (*glyph as u64) & (mask) > 0 
                        {
                            target_value = 0xFFFFFF;
                        }
                        *target_location = target_value;
                        mask >>= 1;
                        line += 4;
                    }
                    let target_location = (self.lfb as *const u8 as u64 + line) as *mut u32;
                    *target_location = 0;
                    glyph = glyph.offset(self.GetGlyphBytesPerLine());
                    offset += self.bytesPerScanline;
                }
            }
            self.currentX += 1;
        }
    }

    fn GetGlyph(&self, c: u8) -> *mut u8
    {
        unsafe
        {
            let glyph_a: *mut u8 = ((self.font) as u64 + (*(self.font)).headersize as u64) as *mut u8;
            let glyph: *mut u8 = glyph_a.offset(
                (if c > 0 && (c as u32) < (*(self.font)).numglyph {
                    c as u32
                } else {
                    0
                } * ((*(self.font)).bytesperglyph)) as isize,
            );

            return glyph
        }
    }

    fn GetGlyphBytesPerLine(&self) -> isize
    {
        unsafe { return (((*self.font).width + 7) / 8) as isize; }
    }
}