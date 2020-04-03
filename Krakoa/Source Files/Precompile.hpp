#pragma once

// STL libraries

// Containers
#include <vector>
#include <sstream>

#if _MSVC_LANG >= 201400L
//	Threading
#	include <future>
#	include <thread>
#endif //_MSVC_LANG >= 201400L

#include <memory>
#include <utility>

// End STL Libraries

// Windows Libraries
#if KRAKOA_OS_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	undef WIN32_LEAN_AND_MEAN
#endif

// kLib Lib
#include <kLib.hpp>

// KRAKOA Lib
#include <Core/Logging/Logger.hpp>
