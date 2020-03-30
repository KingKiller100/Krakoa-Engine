#pragma once

#ifdef KRAKOA_OS_WINDOWS
#	ifdef KRAKOA_BUILD_DLL
#		define KRAKOA_API __declspec(dllexport)
#		define EXPIMP_TEMPLATE 
#	else
#		define KRAKOA_API __declspec(dllimport)
#		define EXPIMP_TEMPLATE extern
#	endif
#else
#	error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#define BIT_SHIFT(n) (1 << n)

// Standard Placeholders
#define PL_HOLDER(x) std::placeholders::_##x

// Binding
#define KRK_BIND1(func) std::bind(&func, this, PL_HOLDER(1))
#define KRK_BIND2(func) std::bind(&func, this, PL_HOLDER(1), PL_HOLDER(2))
