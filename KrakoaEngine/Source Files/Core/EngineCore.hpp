#pragma once

#ifdef KRAKOA_OS_WINDOWS
	#ifdef KRAKOA_BUILD_DLL
		#define KRAKOA_API __declspec(dllexport)
		#define EXPIMP_TEMPLATE 
	#else
		#define KRAKOA_API __declspec(dllimport)
		#define EXPIMP_TEMPLATE extern
	#endif
#else
	#error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif


#if _HAS_CXX17
	#define USE_RESULT [[nodiscard]]
#else
	#define constexpr 	
	#define USE_RESULT 
#endif

#define IS_TRUE 1
#define IS_FALSE 0

#if defined(TEST_ALLOWED)
	#define TESTING_ENABLED 1
#endif // 