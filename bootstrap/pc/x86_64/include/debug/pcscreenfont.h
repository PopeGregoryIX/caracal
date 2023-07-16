#ifndef __PCSCREENFONT__H__
#define __PCSCREENFONT__H__

#include <stdint.h>

/* font */
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

extern "C" volatile unsigned char _binary_pc_x86_64_src_data_font_psf_start;

#endif