#pragma once

// C STD libraries
#include <cstdint>

// STL libraries

// Containers
#include <any>
#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if _MSVC_LANG >= 201400L
//	Threading
#	include <future>
#	include <thread>
#endif //_MSVC_LANG >= 201400L

#include <memory>
#include <utility>
#include <stdexcept>

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
#include "Instrumentor.hpp"
