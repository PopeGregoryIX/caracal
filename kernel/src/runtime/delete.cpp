/**
 * \file delete.cpp
 * \date 26 Oct 2011
 * \author Adam Jones (AJSoft)
 * \brief Defines the delete operator override for the kernel.
 */
#include <stddef.h>

void operator delete(void *p)
{
	(void)p;
}

void operator delete[](void *p)
{
	(void)p;
}



