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


#if _HAS_CXX17
#	include <string_view>
#endif //_MSVC_LANG >= 201702L

#if _MSVC_LANG >= 201400L
// Threading
#	include <future>
#	include <thread>
#endif //_MSVC_LANG >= 201400L

// Input/Output Handlers
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>

// Misc.
#if _MSVC_LANG >= 201400L
	#include <chrono>
#endif //_MSVC_LANG >= 201400L

#include <memory>
#include <sstream>
#include <utility>

// End STL Libraries

// Primitive types handling
#include <cwchar>
#include <type_traits>

// Windows Libraries
#if KRAKOA_OS_WINDOWS
#	if _HAS_CXX17
#		include <corecrt.h>
#		include <corecrt_wdirect.h>
#		include <corecrt_wstdio.h>
#	endif //_MSVC_LANG >= 201702L

#	include <direct.h>
#	include <cstdio>
#	include <Windows.h>
#endif

// KRAKOA Lib

#include <Core/Logger.hpp>
