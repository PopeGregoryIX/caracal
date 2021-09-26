#ifndef __PAGE_FRAME_ALLOCATOR_H
#define __PAGE_FRAME_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>
#include <support/staticbitmap.h>


class PageFrameAllocator
{
    public:
        static PageFrameAllocator& GetInstance( void )  {return _instance;  }

        void Initialise( void );
    private:
        static PageFrameAllocator _instance;

        StaticBitmap _pages;
};

#endif
