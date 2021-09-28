/**
 * @file pageframeallocator.cpp
 * @author Adam Jones (mail@ajsoft.co.uk)
 * @brief Implementation of the Page Frame Allocator.
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include <debug/debug.h>
#include <memory/pageframeallocator.h>
#include <support/staticbitmap.h>
#include <support/string.h>
#include <memory/memoryarray.h>

PageFrameAllocator PageFrameAllocator::_instance;

void PageFrameAllocator::Initialise( size_t frameSize )
{
    _frameSize = ((frameSize > 0x100) && (frameSize % sizeof(uintptr_t) == 0)) ? frameSize : 0x1000;

    MemoryArray& mmap = MemoryArray::GetInstance();
    mmap.Align(_frameSize);
    char decimal[0x20];
    String::itoa(MemoryArray::GetInstance().GetHighestAddress() / (1024*1024), decimal, 10);

    INFO("Total Free Memory: " << decimal << "MiB");

    size_t bitmapSizeBytes = ((MemoryArray::GetInstance().GetHighestAddress() / _frameSize) / sizeof(uint64_t)); // we can store 0x1000 per *bit*
    if(bitmapSizeBytes % sizeof(uint64_t)) bitmapSizeBytes += sizeof(uint64_t) - (bitmapSizeBytes % sizeof(uint64_t));

    uint64_t* bitmapBase = (uint64_t*)mmap.Allocate(bitmapSizeBytes);
    if(bitmapBase == nullptr) FATAL("Unable to allocate memory for Page Frame Allocator");

    //  Assume that all memory is unavailable unless marked otherwise
    _pages = StaticBitmap(bitmapBase, (size_t)bitmapSizeBytes, false, true);
    MemoryArrayIterator i = mmap.GetIterator();
    do
    {
        if(i.Current().IsFree() && (i.Current().size > 0))
            _pages.Clear((i.Current().base / _frameSize), i.Current().size / _frameSize);
    } while (i.MoveNext());
}
