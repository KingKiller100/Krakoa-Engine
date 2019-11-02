#pragma once

#ifdef KRAKOA_OS_WINDOWS
	#ifdef KRAKOA_BUILD_DLL
		#define KRAKOA_API __declspec(dllexport)
	#else
		#define KRAKOA_API __declspec(dllimport)
	#endif
#else
	#error SOORY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#define USE_RESULTS [[nodiscard]]
