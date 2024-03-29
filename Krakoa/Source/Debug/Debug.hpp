#pragma once

#include "DebugCore.hpp"

#include "../Core/EngineMacros.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/Debug/kAssert.hpp>
#include <Utility/Debug/kDebugger.hpp>

#if KRK_ENABLE_ASSERT
#	define KRK_BREAK_IF(cond) if ( (cond) ) { ::klib::kDebug::BreakPoint(); }
#	define KRK_ASSERT(cond, msg) if ( !(cond) ) { ::krakoa::debug::RaiseNotice(msg, SOURCE_INFO()); }

#else
#	define KRK_BREAK_IF(cond) ((void)0)
#	define KRK_ASSERT(cond, msg) kAssertCB(cond, msg, [](const std::string& m, const klib::SourceInfo& s)\
{\
	::krakoa::debug::RaiseException(m, s, krakoa::EngineLogger::GetLogger());\
})\

#endif

#define KRK_FATAL(msg) ::krakoa::debug::RaiseException(msg, SOURCE_INFO())

