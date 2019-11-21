#pragma once

#ifdef KRAKOA_OS_WINDOWS
	#ifdef KRAKOA_BUILD_DLL
		#define KRAKOA_API __declspec(dllexport)
		#define EXPIMP_TEMPLATE 
	#else
		#define KRAKOA_API __declspec(dllimport)
		#define EXPIMP_TEMPLATE extern
	#endif

#define FUNC_SIG __FUNCSIG__
#else
	#error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#if _HAS_CXX17
	#define USE_RESULT [[nodiscard]]
#else
	#define USE_RESULT 
#endif
