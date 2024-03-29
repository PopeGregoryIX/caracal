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
#include <memory/pageframeallocator.h>
#include <structures/staticbitmap.h>
#include <memory/memoryarray.h>

PageFrameAllocator PageFrameAllocator::_instance;

void PageFrameAllocator::Initialise( size_t frameSize, MemoryArray& memoryArray, uintptr_t virtualLocation  )
{
    _frameSize = ((frameSize > 0x100) && (frameSize % sizeof(uintptr_t) == 0)) ? frameSize : 0x1000;

    char decimal[0x20];
    itoa(memoryArray.GetHighestAddress() / (1024*1024), decimal, 10);
    //INFO("Total Free Memory: " << decimal << "MiB");

    size_t bitmapSizeBytes = ((memoryArray.GetHighestAddress() / _frameSize) / sizeof(uint64_t)); // we can store 0x1000 per *bit*
    if(bitmapSizeBytes % sizeof(uint64_t)) bitmapSizeBytes += sizeof(uint64_t) - (bitmapSizeBytes % sizeof(uint64_t));

    uint64_t* bitmapBase = (uint64_t*)virtualLocation;
    if(bitmapBase == nullptr) FATAL("Unable to allocate memory for Page Frame Allocator");

    //  Assume that all memory is unavailable unless marked otherwise
    _pages = StaticBitmap(bitmapBase, (size_t)bitmapSizeBytes, false, true);

    MemoryMapEntry* mme = memoryArray.GetFirst();
    for(size_t i = 0; i < memoryArray.Count(); i++)
    {
        if (mme[i].IsFree() && mme[i].size > 0)
        {
            _pages.Clear((mme[i].base / _frameSize), mme[i].size / _frameSize);
        }
    }
}