/**
 * \file __cxa_atexit.cpp
 * \date 26 Oct 2011
 * \author Adam Jones (AJSoft)
 * \brief Defines the __cxa_atexit support function for global/static objects.
 */

#include <cxx.h>
extern AtExitFunction* __atexit_funcs;

int __cxa_atexit(void (*dtor) (void*), void* arg, void* dso)
{
	if (__atexitFunctionCount >= ATEXIT_FUNCTION_MAX) {return -1;};
		__atexit_funcs[__atexitFunctionCount].dtor = dtor;
		__atexit_funcs[__atexitFunctionCount].arg = arg;
		__atexit_funcs[__atexitFunctionCount].dso = dso;
		__atexitFunctionCount++;
		return 0;
}

