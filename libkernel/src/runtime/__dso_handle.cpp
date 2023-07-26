/**
 * \file __dso_handle.cpp
 * \date 26 Oct 2011
 * \author Adam Jones (AJSoft)
 * \brief Defines the _dso_handle pointer along with other support structures for Global/Static
 * object support.
 */
#include <cxx.h>

AtExitFunction __atexit_funcs[ATEXIT_FUNCTION_MAX];

unsigned int __atexitFunctionCount = 0;

//void *__dso_handle; // This is now provided by crti.o, crtbegin.o, crtend.o, crtn.o
