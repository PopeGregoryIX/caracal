#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include <memory/pageframeallocator.h>
#include <support/staticbitmap.h>
#include <memory/memoryarray.h>

PageFrameAllocator PageFrameAllocator::_instance;

void PageFrameAllocator::Initialise( void )
{
    MemoryArray& mmap = MemoryArray::GetInstance();
    mmap.Align(0x1000);
    INFO("Total Free Memory: " << (uint64_t)(MemoryArray::GetInstance().GetHighestAddress() / (1024 * 1024 * 1024)) << "GiB");

    size_t bitmapSizeBytes = ((MemoryArray::GetInstance().GetHighestAddress() / 0x1000) / sizeof(uint64_t)); // we can store 0x1000 per *bit*
    uint64_t* bitmapBase = (uint64_t*)mmap.Allocate(bitmapSizeBytes);
    if(bitmapBase == nullptr) FATAL("Unable to allocate memory for Page Frame Allocator");

    _pages = StaticBitmap(bitmapBase, (size_t)bitmapSizeBytes, true);
    
    mmap.Print();

    // Mark all unavailable memory map pages as unavailable in the page frame allocator.
    MemoryArrayIterator i = mmap.GetIterator();
    do
    {
        if(!i.Current().IsFree() && (i.Current().size > 0))
            _pages.Set((i.Current().base / 0x1000), i.Current().size / 0x1000);
    } while (i.MoveNext());
}
