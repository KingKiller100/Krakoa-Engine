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
#	include <Windows.h>
#endif

// kLib Lib
#include <kLib.hpp>

// KRAKOA Lib
//#include <Core/Logger.hpp>
