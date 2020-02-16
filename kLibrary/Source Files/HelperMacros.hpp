#pragma once

#if KLIB_LIB
	#define KLIB_API __declspec(dllexport)
	#define PORT_STL
#else
	#define KLIB_API __declspec(dllimport)
	#define PORT_STL extern
#endif

#if _MSVC_LANG > 201402L
	#define USE_RESULT [[nodiscard]]
#else
	#define noexcept throw ()
	#define constexpr 	
	#define USE_RESULT 
#endif

#define IS_TRUE 1
#define IS_FALSE 0

#if defined(KLIB_TEST)
	#define TESTING_ENABLED 1
#endif // 