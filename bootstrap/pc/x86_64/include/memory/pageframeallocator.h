/**
 * @file pageframeallocator.h
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Contains the PageFrameAllocator class.
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PAGE_FRAME_ALLOCATOR_H
#define __PAGE_FRAME_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>
#include <support/staticbitmap.h>
#include <debug/debug.h>
#include <memory/spinlock.h>

/**
 * @brief Responsible for allocating frames of physical RAM.
 * 
 * This class currently uses a top-down flat bitmap approach.
 */
class PageFrameAllocator
{
    private:
        static PageFrameAllocator _instance;
        StaticBitmap _pages;
        size_t _frameSize;

        inline uintptr_t GetWholePages(size_t bytes) 
        { 
            size_t pages = bytes / _frameSize; 
            if((bytes % _frameSize) !=0) pages++;
            return pages; 
        }
    public:
        static PageFrameAllocator& GetInstance( void )  { return _instance;  }

        void Initialise( size_t frameSize );

        ///TODO: Needs a lock!
        inline uintptr_t Allocate( void )   {   return _pages.FindAndSet() * _frameSize; }

        inline uintptr_t Allocate(size_t bytes) 
        {
            uintptr_t returnValue = _pages.FindAndSet(GetWholePages(bytes));
            return (returnValue == UINT64_MAX) ? UINT64_MAX : returnValue * 0x1000;
        }

        inline uintptr_t Allocate(size_t bytes, size_t alignment) 
        {
            uintptr_t returnValue = _pages.FindAndSet(GetWholePages(bytes), GetWholePages(alignment));
            return (returnValue == UINT64_MAX) ? UINT64_MAX : returnValue * 0x1000;
        }

        inline void Free( uintptr_t page ) { _pages.Clear(page / _frameSize); }

        inline void Free( uintptr_t page, size_t bytes ) { _pages.Clear(page / _frameSize, GetWholePages(bytes)); }
};

#endif
