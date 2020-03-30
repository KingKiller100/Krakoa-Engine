#pragma once

#if KLIB_LIB
#	define KLIB_API __declspec(dllexport)
#	define PORT_LIB
#else
#	define KLIB_API __declspec(dllimport)
#	define PORT_LIB extern
#endif

#if _MSVC_LANG > 201402L
#	define USE_RESULT [[nodiscard]]
#else
#	define USE_RESULT 
#	if MSVC_PLATFORM_TOOLSET < 110
#		define noexcept throw ()
#		define constexpr 
#	endif
#endif

#define IS_TRUE 1
#define IS_FALSE 0

#define TO_TYPE(type, val) static_cast<type>(val)

#if defined(KLIB_TEST)
#	define TESTING_ENABLED 1
#endif // KLIB_TEST