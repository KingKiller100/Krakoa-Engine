#pragma once

#include <HelperMacros.hpp>
#include <Testing/EnableTesting.hpp>

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

// Binding
#define KRK_BIND_FUNC(func) \
	[this](auto&& ...params) -> decltype(auto) { return this->func(std::forward<decltype(params)>(params)...);} \

#ifndef KRAKOA_PROFILE
#	ifndef KRK_ENABLE_STATISTICS
#		define KRK_ENABLE_STATISTICS KRK_TRUE
#	endif
#endif

#ifndef KRK_ENABLE_ASSERT
# define KRK_ENABLE_ASSERT KRK_TRUE
#endif

