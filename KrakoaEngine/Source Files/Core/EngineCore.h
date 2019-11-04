#pragma once

#include <vcruntime.h>

#ifdef KRAKOA_OS_WINDOWS
	#ifdef KRAKOA_BUILD_DLL
		#define KRAKOA_API __declspec(dllexport)
	#else
		#define KRAKOA_API __declspec(dllimport)
	#endif
#else
	#error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#if _HAS_CXX17
	#define USE_RESULT [[nodiscard]]
#else
	#define USE_RESULT 
#endif
