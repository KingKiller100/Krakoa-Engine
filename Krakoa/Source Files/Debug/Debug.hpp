#pragma once

#include "../Core/Logging/CoreLogger.hpp"
#include "../Core/Logging/MemoryLogger.hpp"

#include <Utility/Debug/kAssert.hpp>
#include <Utility/Debug/kDebugger.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>

namespace krakoa::debug
{
	void RaiseException(const std::string& msg, const klib::SourceInfo& sourceInfo, klib::Logging& logger);
}

#define KRK_ASSERT(cond, msg) kAssertCB(cond, msg, [](const std::string& m, const klib::SourceInfo& s)\
{\
	krakoa::debug::RaiseException(m, s, krakoa::CoreLogger::GetLogger());\
})\

#define MEM_ASSERT(cond, msg) kAssertCB(cond, msg, [](const std::string& m, const klib::SourceInfo& s)\
{\
	krakoa::debug::RaiseException(m, s, memory::MemoryLogger::GetLogger());\
})\

#define KRK_FATAL(msg) krakoa::debug::RaiseException(msg, SOURCE_INFO(), krakoa::CoreLogger::GetLogger())
#define MEM_FATAL(msg) krakoa::debug::RaiseException(msg, SOURCE_INFO(), memory::MemoryLogger::GetLogger())


#if defined(KRAKOA_DEBUG) || defined(KRAKOA_TEST)
#	define KRK_BREAK_IF(cond) if ( (cond) ) { klib::kDebug::BreakPoint(); }
#else
#	define KRK_BREAK_IF(cond) ((void)0)
#endif