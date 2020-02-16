#pragma once

// STL libraries

// Containers
#include <array>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <string>
#include <vector>


#if MSVC_PLATFORM_TOOLSET >= 141
	#include <string_view>
#endif //MSVC_PLATFORM_TOOLSET >= 141

// Threading
#if MSVC_PLATFORM_TOOLSET >= 140
	#include <future>
#endif //MSVC_PLATFORM_TOOLSET >= 140

#if MSVC_PLATFORM_TOOLSET >= 110
	#include <thread>
#endif //MSVC_PLATFORM_TOOLSET >= 140

// Input/Output Handlers
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>

// Misc.
#if MSVC_PLATFORM_TOOLSET >= 110
#include <chrono>
#endif //MSVC_PLATFORM_TOOLSET >= 110

#include <memory>
#include <sstream>
#include <utility>

// End STL Libraries

// Primitive types handling
#include <cwchar>
#include <type_traits>

// Windows Libraries
#if KLIB_WINDOWS_OS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#if MSVC_PLATFORM_TOOLSET >= 110
	#include <corecrt.h>
	#include <corecrt_wdirect.h>
	#include <corecrt_wstdio.h>
	#endif //MSVC_PLATFORM_TOOLSET >= 141
#endif
	
#include <direct.h>
#include <cstdio>
#include <Windows.h>

