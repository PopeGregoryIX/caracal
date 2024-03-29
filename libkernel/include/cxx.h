/**
 * \file cxx.h
 * \author Adam Jones (AJSoft)
 * \date 26 Oct 2011
 * \brief Support code such as ABI defined linkage for language features.
 *
 * This class provides support declarations such as support for Global and
 * Static objects along with support for Pure Virtual Functions.
 *
 * \todo todo:By initialising global/static objects after the heap setup, we can avoid
 * the artificial limit on global objects.
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <stdint.h>
#include <stddef.h>

extern "C"
{

int idle(int, char**);

//	Library functions (until replaced by kernel-level stdlib)
extern "C" void _init( void );
extern "C" void _fini( void );
extern "C" void* memset( void* ptr, int value, size_t num);
extern "C" void* memcpy( void* dest, void* src, size_t numBytes);
int strncmp(const char* s1, const char* s2, int maxLength);
int strcmp(const char* s1, const char* s2);
size_t strlen(const char* s);
char* itoa(uint64_t num, char* buffer, uint8_t base);
char* strcpy(char* dest, const char* src);
size_t octToBin(unsigned char *str, int size) ;

//	Support for Global / Static Objects////////////////////////////////
#define ATEXIT_FUNCTION_MAX	0x200 ///< The maximum number of AtExit functions that can be supported.

/**
 * \brief Register a class destructor.
 *
 * Registers the destructor of a static/global class to be called during finalisation (ABI).
 *
 * \param dtor A function pointer to the destructor.
 * \param arg The argument to be passed to the object destructor.
 * \param dso ABI-defined handle.
 */
int __cxa_atexit(void (*dtor) (void*), void* arg, void* dso);

//	ARM EABI equivalent.
int __aeabi_atexit(void* object, void (*destroyer)(void*), void* dso_handle);

/**
 * \brief Finalize a single/all global and static objects.
 *
 * \param f Object to finalize, NULL to finalize all objects.
 */
void __cxa_finalize(void* f);

/**
 * \brief The number of AtExit functions currently registered.
 */
extern unsigned int __atexitFunctionCount;

/**
 * \brief ABI-defined handle required for global/static class support.
 */
extern void* __dso_handle;

/**
 * \brief Defines a destructor called by __cxa_finalize.
 */
struct AtExitFunction
{
	void (*dtor)(void *); 	///< Destructor function to be called on object finalization.
	void *arg;				///< Argument to be passed to the destructor.
	void *dso;				///< DSO handle defined by the System V ABI.
};
///////////////////////////////////////////////////////////////////////

//	Support for Pure Virtual Functions/////////////////////////////////
/**
 * \brief Called whenever a pure virtual function which has not been defined is called.
 *
 * This function should never be called - the compiler will pick up our error!
 */
void __cxa_pure_virtual();
///////////////////////////////////////////////////////////////////////

}


//	Memory Copy Template and Specializations///////////////////////////
/**
 * \brief The memory copy template function.
 *
 * This function in all its forms should be safe for aligned, overlapping
 * memory areas.
 *
 * \param dest Destination memory address to copy to.
 * \param src Source memory address to copy from.
 * \param count Number of instances of T to copy.
 *
 */
template <class TData> TData* memorycopy(const TData* dest, const TData* src, size_t count)
{
    TData* d = (TData*)dest, * s = (TData*)src;

    //	This if statement makes safe for overlapping memory areas.
    if(d >= s)
        while(count--) d[count] = s[count];
    else
        while(count--) *d++ = *s++;

    return (TData*)dest;
}

///////////////////////////////////////////////////////////////////////

#endif /* SUPPORT_H_ */
