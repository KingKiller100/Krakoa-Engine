#pragma once

#if KLIB_LIB
	#define KLIB_API __declspec(dllexport)
	#define EXPORT_STL
#else
	#define KLIB_API __declspec(dllimport)
	#define EXPORT_STL extern
#endif

#if _HAS_CXX17
	#define USE_RESULT [[nodiscard]]
#else
	#define noexcept throw ()
	#define constexpr 	
	#define USE_RESULT 
#endif

#define IS_TRUE 1
#define IS_FALSE 0