#pragma once

#if KLIB_LIB
//#	if KLIB_DYNAMIC_LINK
//#		ifdef KLIB_BUILD_DLL
//#			define KLIB_API __declspec(dllexport)
//#			define PORT_LIB
//#		else
//#			define KLIB_API __declspec(dllimport)
//#			define PORT_LIB extern
//#		endif
//#	else
//#		define KLIB_API 
//#		define PORT_LIB 
//#	endif
#endif

#if _MSVC_LANG > 201402L
#	define UNUSED [[maybe_unused]]
#	define USE_RESULT [[nodiscard]]
#else
#	define UNUSED 
#	define USE_RESULT 
#	if MSVC_PLATFORM_TOOLSET < 110
#		define noexcept throw ()
#		define constexpr 
#	endif
#endif

#define KLIB_TRUE 1
#define KLIB_FALSE 0

#define CAST(type, val) static_cast<type>(val)
#define REINTERPRET(type, val) reinterpret_cast<type>(val)

#define ONLY_TYPE(type) std::decay_t<std::remove_pointer_t<type>>


#if defined(KLIB_TEST)
#	define TESTING_ENABLED 1
#endif // KLIB_TEST
