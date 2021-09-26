#include <stdint.h>
#include <stddef.h>
#include <runtime/cxx.h>
#include <support/staticbitmap.h>
#include <debug/debug.h>

StaticBitmap::StaticBitmap(uint64_t* bitmap, size_t bytes, bool zero)
: _bitmap(bitmap), _bytes(bytes)
{
    if(zero) memset(bitmap, 0, bytes);
}

void StaticBitmap::Set(size_t base, size_t length)
{
    if(base > Max() || (base + length - 1) > Max()) FATAL("Overflow detected on StaticBitmap");


    size_t startQuad = base / BITS_PER_QUAD;
    size_t endQuad = (base + length - 1) / BITS_PER_QUAD;

    size_t startIndex = base % BITS_PER_QUAD;
    size_t endIndex = (base + length - 1) % BITS_PER_QUAD;

    if(startQuad == endQuad)
    {
        if(startIndex == endIndex)
            _bitmap[startQuad] |= (1ULL << startIndex);
        else
        {
            
            for(size_t i = startIndex; i <= endIndex; ++i)
                _bitmap[startQuad] |= (1ULL << i);
        }
    }
    else
    {
        for(size_t q = startQuad; q <= endQuad; ++q)
        {
            if(q == startQuad)
            {
                for(size_t i = startIndex; i < BITS_PER_QUAD; ++i) 
                    _bitmap[q] |= (1ULL << i);
            }
            else if(q == endQuad)
            {
                for(size_t i = 0; i < endIndex; ++i)
                {
                    _bitmap[q] |= (1ULL << i);
                }
            }
            else
                _bitmap[q] = UINT64_MAX;
        }
    }
}