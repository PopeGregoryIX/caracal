#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include <memory/pageframeallocator.h>
#include <support/staticbitmap.h>
#include <memory/memorymap.h>

PageFrameAllocator PageFrameAllocator::_instance;

void PageFrameAllocator::Initialise( void )
{
    MemoryMap& mmap = MemoryMap::GetInstance();
    mmap.Align(0x1000);
    INFO("Total Free Memory: " << (uint64_t)MemoryMap::GetInstance().GetHighestAddress());

    size_t bitmapSize = ((MemoryMap::GetInstance().GetHighestAddress() / 0x1000) / sizeof(uint64_t) / 8); // we can store 0x1000 per *bit*
    uint64_t* bitmapBase = (uint64_t*)mmap.Allocate(bitmapSize);
    if(bitmapBase == nullptr) FATAL("Unable to allocate memory for Page Frame Allocator");

    _pages = StaticBitmap(bitmapBase, (size_t)bitmapSize, true);

    uint64_t a[2];

    StaticBitmap sbmp((uint64_t*)a, (size_t)16, true);

    INFO(sbmp._bitmap[0] << " : " << sbmp._bitmap[1]);
    sbmp.Set(32,97);
    INFO(sbmp._bitmap[0] << " : " << sbmp._bitmap[1]);

    //mmap.Print();
}
