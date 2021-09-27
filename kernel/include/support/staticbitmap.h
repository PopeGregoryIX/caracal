/**
 * @file staticbitmap.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief A bitmap manipulation class which does not provide dynamic reallocation
 * of its data structures.
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __BITMAP_H_
#define __BITMAP_H_

#include <stdint.h>
#include <debug/debug.h>

class StaticBitmap
{
    public:
        StaticBitmap() : _bitmap(nullptr), _bytes(0){}

        StaticBitmap(uint64_t* bitmap, size_t bytes, bool zero);

        void Clear(size_t base, size_t length);
        inline void Clear(size_t bit) 
        {   
            if(bit > Max()) FATAL("Overflow detected on StaticBitmap while clearing bits.");
            _bitmap[bit / BITS_PER_QUAD] &= ~(1ULL << (bit % BITS_PER_QUAD));    
        }

        void Set(size_t base, size_t length);
        inline void Set(size_t bit) 
        { 
            if(bit > Max()) FATAL("Overflow detected on StaticBitmap while setting bits.");
            _bitmap[bit / BITS_PER_QUAD] |= (1ULL << (bit % BITS_PER_QUAD));    
        }

        uint64_t FindFirstClear();
        uint64_t FindFirstClear(size_t length);
        uint64_t FindLastClear();
        uint64_t FindLastClear(size_t length);
        
        inline size_t Max(void){  return (_bytes * sizeof(uint64_t)) - 1; }

        void operator=(StaticBitmap bmp) { _bitmap = bmp._bitmap; _bytes = bmp._bytes; }
    
        uint64_t* _bitmap;
    
    private:
        size_t _bytes;

        const size_t BITS_PER_QUAD = sizeof(uint64_t) * 8;
};

#endif