#ifndef __BITMAP_H_
#define __BITMAP_H_

#include <stdint.h>

class StaticBitmap
{
    public:
        StaticBitmap() : _bitmap(nullptr), _bytes(0){}

        StaticBitmap(uint64_t* bitmap, size_t bytes, bool zero);

        void Clear(size_t base, size_t length);
        void Set(size_t base, size_t length);

        inline size_t Max(void){  return _bytes * sizeof(uint64_t); }

        void operator=(StaticBitmap bmp) { _bitmap = bmp._bitmap; _bytes = bmp._bytes; }
    
    uint64_t* _bitmap;
    
    private:
        
        size_t _bytes;

        const size_t BITS_PER_QUAD = sizeof(uint64_t) * 8;
};

#endif