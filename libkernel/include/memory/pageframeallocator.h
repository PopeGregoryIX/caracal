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
#include <cxx.h>
#include <structures/staticbitmap.h>
#include <memorylayout.h>

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
        Spinlock _lock;

        inline uintptr_t GetWholePages(size_t bytes) 
        { 
            size_t pages = bytes / _frameSize; 
            if((bytes % _frameSize) !=0) pages++;
            return pages; 
        }
    public:
        static PageFrameAllocator& GetInstance( void )  { return _instance;  }

        void Initialise( size_t frameSize, class MemoryArray& memoryArray, uintptr_t virtualLocation );

        ///TODO: Needs a lock!
        inline uintptr_t Allocate( void )   
        {   
            _lock.Acquire();
            uintptr_t alloc = _pages.FindAndSet() * _frameSize;
            _lock.Release();
            return alloc;
        }

        inline uintptr_t AllocateEmpty( void )
        {
            uintptr_t alloc = Allocate();
            if(alloc != UINT64_MAX) memset(GET_VIRTUAL_POINTER(alloc), 0, _frameSize);
            return alloc;
        }

        inline uintptr_t Allocate(size_t bytes) 
        {
            _lock.Acquire();
            uintptr_t returnValue = _pages.FindAndSet(GetWholePages(bytes));
            _lock.Release();
            return (returnValue == UINT64_MAX) ? UINT64_MAX : returnValue * 0x1000;
        }

        inline uintptr_t AllocateEmpty(size_t bytes) 
        {
            uintptr_t alloc = Allocate(bytes);
            if(alloc != UINT64_MAX) memset(GET_VIRTUAL_POINTER(alloc), 0, bytes);
            return alloc;
        }

        inline uintptr_t Allocate(size_t bytes, size_t alignment) 
        {
            _lock.Acquire();
            uintptr_t returnValue = _pages.FindAndSet(GetWholePages(bytes), GetWholePages(alignment));
            _lock.Release();
            return (returnValue == UINT64_MAX) ? UINT64_MAX : returnValue * 0x1000;
        }

        inline void Free( uintptr_t page ) 
        { 
            _lock.Acquire();
            _pages.Clear(page / _frameSize); 
            _lock.Release();
        }

        inline void Free( uintptr_t page, size_t bytes ) 
        { 
            _lock.Acquire();
            _pages.Clear(page / _frameSize, GetWholePages(bytes)); 
            _lock.Release();
        }
};

#endif