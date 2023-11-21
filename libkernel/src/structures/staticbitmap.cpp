/**
 * @file staticbitmap.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Implementation of the static bitmap class.
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include <cxx.h>
#include <structures/staticbitmap.h>
#include <debug.h>

StaticBitmap::StaticBitmap(uint64_t* bitmap, size_t bytes, bool zero, bool max)
: _bitmap(bitmap), _bytes(bytes)
{
    if(zero && max) FATAL("Cannot initialise Bitmap to both zero and 1!");
    if(zero) memset(bitmap, 0, _bytes);
    if(max) memset(bitmap, -1, _bytes);
}

void StaticBitmap::Clear(size_t base, size_t length)
{
    _lock.Acquire();
    if(base > Max() || (base + length - 1) > Max()) 
        FATAL("Overflow detected on StaticBitmap while clearing bits.");
    

    size_t startQuad = base / BITS_PER_FRAME;
    size_t endQuad = (base + length - 1) / BITS_PER_FRAME;

    size_t startIndex = base % BITS_PER_FRAME;
    size_t endIndex = (base + length - 1) % BITS_PER_FRAME;
    
    if(startQuad == endQuad)
    {
        uint64_t bitPattern = 0;
        if(startIndex == endIndex)
            bitPattern |= (1ULL << startIndex);
        else
        {
            
            for(size_t i = startIndex; i <= endIndex; ++i)
                bitPattern |= (1ULL << i);
        }

        _bitmap[startQuad] &= ~bitPattern;
    }
    else
    {
        for(size_t q = startQuad; q <= endQuad; ++q)
        {
            uint64_t bitPattern = 0;
            if(q == startQuad)
            {
                for(size_t i = startIndex; i < BITS_PER_FRAME; ++i) 
                     bitPattern |= (1ULL << i);
            }
            else if(q == endQuad)
            {
                for(size_t i = 0; i <= endIndex; ++i)
                    bitPattern |= (1ULL << i);
            }
            else
                bitPattern = UINT64_MAX;

            _bitmap[q] &= ~bitPattern;
        }
    }
    _lock.Release();
}

uint64_t StaticBitmap::FindFirstClear()
{
    _lock.Acquire();
    for(size_t i = 0; i < (_bytes / sizeof(uint64_t)); ++i)
    {
        if(_bitmap[i] != UINT64_MAX)
        {
            for(size_t j = 0; j < BITS_PER_FRAME; ++j)
            {
                if(((_bitmap[i] ^ (1ULL << j)) & (1ULL << j)) == (1ULL << j))
                {
                    _lock.Release();
                    return (i * BITS_PER_FRAME) + j;
                }
            }
        }
    }

    _lock.Release();
    return -1;
}

uint64_t StaticBitmap::FindLastClear()
{
    _lock.Acquire();
    for(size_t i = (_bytes / sizeof(uint64_t)) - 1; i != UINT64_MAX; --i)
    {
        if(_bitmap[i] != UINT64_MAX)
        {
            size_t bitPattern = (1ULL << (BITS_PER_FRAME - 1));

            for(size_t j = 0; j <= (BITS_PER_FRAME - 1); ++j)
            {
                if(((_bitmap[i] ^ bitPattern) & bitPattern) == bitPattern)
                {
                    _lock.Release();
                    return (i * BITS_PER_FRAME) + (BITS_PER_FRAME - (j + 1));
                }
                bitPattern = bitPattern >> 1;
            }
        }
    }

    _lock.Release();
    return -1;
}

uint64_t StaticBitmap::FindFirstClear(size_t length)
{
    _lock.Acquire();
    size_t frames = length / BITS_PER_FRAME;
    if((length % BITS_PER_FRAME) != 0) frames++;

    for(size_t i = 0; i <= (_bytes / sizeof(uint64_t)) - frames; ++i)
    {
        if(frames > 1)
        {
            // for speed find by whole frames only.
            bool found = true;
            for(size_t j = 0; j < frames; ++j)
                if(_bitmap[i + j] != 0) found = false;

            //  we have found the start of enough consecutive frames for allocation!
            if (found) 
            {
                _lock.Release();
                return (i * BITS_PER_FRAME);
            }
        }
        else
        {
            // everything we need can be found in a single frame
            size_t bitPattern = 0;
            for(size_t j = 0; j < length; ++j) bitPattern |= (1ULL << j);

            for(size_t j = 0; j <= (BITS_PER_FRAME - length); ++j)
            {
                if(((_bitmap[i] ^ bitPattern) & bitPattern) == bitPattern)
                {
                    _lock.Release();
                    return (i * BITS_PER_FRAME) + j;
                }
                bitPattern = bitPattern << 1;
            }
        }
    }

    _lock.Release();
    return -1;
}

uint64_t StaticBitmap::FindLastClear(size_t length)
{
    _lock.Acquire();
    size_t frames = length / BITS_PER_FRAME;
    if((length % BITS_PER_FRAME) != 0) frames++;

    for(size_t i = (_bytes / sizeof(uint64_t)) - frames; i != UINT64_MAX; --i)
    {
        if(frames > 1)
        {
            // for speed find by whole frames only.
            bool found = true;
            for(size_t j = 0; j < frames; ++j)
                if(_bitmap[i + j] != 0) found = false;

            //  we have found the start of enough consecutive frames for allocation!
            if (found) 
            {
                _lock.Release();
                return (i * BITS_PER_FRAME);
            }
        }
        else
        {
            // everything we need can be found in a single frame
            size_t bitPattern = 0;
            for(size_t j = BITS_PER_FRAME - length; j < BITS_PER_FRAME; ++j) bitPattern |= (1ULL << j);

            for(size_t j = 0; j <= (BITS_PER_FRAME - length); ++j)
            {
                if(((_bitmap[i] ^ bitPattern) & bitPattern) == bitPattern)
                {
                    _lock.Release();
                    return (i * BITS_PER_FRAME) + (BITS_PER_FRAME - (j + length));
                }
                bitPattern = bitPattern >> 1;
            }
        }
    }
    _lock.Release();
    return -1;
}

uint64_t StaticBitmap::FindLastClear(size_t length, size_t alignment)
{
    _lock.Acquire();
    size_t frames = length / BITS_PER_FRAME;
    if((length % BITS_PER_FRAME) != 0) frames++;
    
    size_t alignFrames = alignment / BITS_PER_FRAME;
    if(alignment % BITS_PER_FRAME) alignFrames++;

    size_t topFrame = (_bytes / sizeof(uint64_t)) - frames;
    topFrame-= topFrame % alignFrames;

    for(size_t i = topFrame; i != UINT64_MAX; i-= alignFrames)
    {
        // for speed find by whole frames only.
        bool found = true;
        for(size_t j = 0; j < frames; ++j)
            if(_bitmap[i + j] != 0) found = false;

        //  we have found the start of enough consecutive frames for allocation!
        if (found)
        {
            _lock.Release();
             return (i * BITS_PER_FRAME);
        }
    }
    _lock.Release();
    return -1;
}

void StaticBitmap::Set(size_t base, size_t length)
{
    _lock.Acquire();
    if(base > Max() || (base + length - 1) > Max())         
    {
        INFO("Base: " << base << "\tLength: " << length);
        
        FATAL("Overflow detected on StaticBitmap while setting bits.");
    }
    size_t startQuad = base / BITS_PER_FRAME;
    size_t endQuad = (base + length - 1) / BITS_PER_FRAME;

    size_t startIndex = base % BITS_PER_FRAME;
    size_t endIndex = (base + length - 1) % BITS_PER_FRAME;

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
                for(size_t i = startIndex; i < BITS_PER_FRAME; ++i) 
                    _bitmap[q] |= (1ULL << i);
            }
            else if(q == endQuad)
            {
                for(size_t i = 0; i <= endIndex; ++i)
                    _bitmap[q] |= (1ULL << i);
            }
            else
                _bitmap[q] = UINT64_MAX;
        }
    }
    _lock.Release();
}