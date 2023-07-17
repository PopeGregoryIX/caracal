/**
 * \file __cxa_finalize.cpp
 * \date 26 Oct 2011
 * \author Adam Jones (AJSoft)
 * \brief Defines the __cxa_finalize support function for global/static objects.
 */
#include <runtime/cxx.h>
#include <stdint.h>
extern AtExitFunction* __atexit_funcs;

void __cxa_finalize(void *f)
{
	if (!f)
	{
		unsigned int i = __atexitFunctionCount;
		while (--i)
		{
			if (__atexit_funcs[i].dtor)
			{
				(*__atexit_funcs[i].dtor)(__atexit_funcs[i].arg);
			};
		};
		return;
	};

	for (unsigned int i = 0 ; i < __atexitFunctionCount; )
	{
		if ((uint64_t)(__atexit_funcs[i].dtor) == (uint64_t)f)
		{
			(*__atexit_funcs[i].dtor)(__atexit_funcs[i].arg);
			__atexit_funcs[i].dtor = 0;
		};
	};
}

