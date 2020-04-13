#pragma once

#ifdef KRAKOA_OS_WINDOWS
#	if KRAKOA_DYNAMIC_LINK
#		ifdef KRAKOA_BUILD_DLL
#			define KRAKOA_API __declspec(dllexport)
#			define EXPIMP_TEMPLATE 
#		else
#			define KRAKOA_API __declspec(dllimport)
#			define EXPIMP_TEMPLATE extern
#		endif
#	else
#		define KRAKOA_API 
#		define EXPIMP_TEMPLATE 
#endif
#else
#	error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#define KRK_TRUE KLIB_TRUE
#define KRK_FALSE KLIB_FALSE

#define BIT_SHIFT(n) (1 << n)

// Standard Placeholders
#define PL_HOLDER(x) std::placeholders::_##x

// Binding
#define KRK_BIND1(func) std::bind(&func, this, PL_HOLDER(1))
#define KRK_BIND2(func) std::bind(&func, this, PL_HOLDER(1), PL_HOLDER(2))

#define ANY_CAST(dest, source) std::any_cast<dest>(source)
