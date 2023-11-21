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
#include <stddef.h>
#include <debug.h>
#include <memory/spinlock.h>



class StaticBitmap
{
     /// Number of bytes used to store each frame
    #define BYTES_PER_FRAME (sizeof(uint64_t))
    #define BITS_PER_FRAME (BYTES_PER_FRAME * 8)

    private:
        uint64_t* _bitmap;                                  ///< Pointer to the actual bitmap data
        size_t _bytes;                                      ///< Total size of bitmap in bytes       
        Spinlock _lock;

    public:
        StaticBitmap() : _bitmap(nullptr), _bytes(0){}

        StaticBitmap(uint64_t* bitmap, size_t bytes, bool zero, bool max);

        inline uint64_t* Bitmap( void ) { return _bitmap; }

        inline size_t Bytes( void ) { return _bytes; }

        void Clear(size_t base, size_t length);
        
        inline void Clear(size_t bit) 
        {   
            _lock.Acquire();
            if(bit > Max()) FATAL("Overflow detected on StaticBitmap while clearing bits.");
            _bitmap[bit / BITS_PER_FRAME] &= ~(1ULL << (bit % BITS_PER_FRAME));    
            _lock.Release();
        }

        inline uint64_t FindAndSet( void ) 
        { 
            uint64_t returnValue = FindLastClear(); 
            if(returnValue != UINT64_MAX) Set(returnValue); 
            return returnValue; 
        }

        inline uint64_t FindAndSet( size_t length ) { uint64_t returnValue = FindLastClear(length); if(returnValue != UINT64_MAX) Set(returnValue, length); return returnValue; }

        inline uint64_t FindAndSet( size_t length, size_t alignment ) 
        { 
            uint64_t returnValue = FindLastClear(length, alignment); 
            if(returnValue != UINT64_MAX) Set(returnValue, length); 
            return returnValue; 
        }

        uint64_t FindFirstClear( void );
        
        uint64_t FindFirstClear(size_t length);
        
        uint64_t FindLastClear( void );
        
        uint64_t FindLastClear(size_t length);

        uint64_t FindLastClear(size_t length, size_t alignment);

        void Set(size_t base, size_t length);
        
        inline void Set(size_t bit) 
        { 
            _lock.Acquire();
            if(bit > Max()) FATAL("Overflow detected on StaticBitmap while setting bits.");
            _bitmap[bit / BITS_PER_FRAME] |= (1ULL << (bit % BITS_PER_FRAME));    
            _lock.Release();
        }
        
        inline size_t Max(void){  return (_bytes * sizeof(uint64_t)) - 1; }

        void operator=(StaticBitmap bmp) { _bitmap = bmp._bitmap; _bytes = bmp._bytes; }
};

#endif