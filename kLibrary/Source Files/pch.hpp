#pragma once

// STL libraries

// Containers
#include <functional>
#include <string>
#include <vector>

#if MSVC_PLATFORM_TOOLSET >= 141
	#include <string_view>
#endif //MSVC_PLATFORM_TOOLSET >= 141

// Input/Output Handlers
#include <iostream>

// Memory
#include <memory>
#include <utility>

// End STL Libraries

// Windows Libraries
#if KLIB_WINDOWS_OS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <Windows.h>
#endif
