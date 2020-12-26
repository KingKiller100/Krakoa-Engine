#pragma once

#include <Utility/Debug/Source/kSourceInfo.hpp>
#include <Utility/Debug/kDebugger.hpp>

namespace krakoa::debug
{
	void FatalError(const std::string_view& msg, const klib::SourceInfo& sourceInfo);
}

#define KRK_FATAL(msg) krakoa::debug::FatalError(msg, SOURCE_INFO());

#if defined(KRAKOA_DEBUG) || defined(KRAKOA_TEST)
#define KRK_WARN(cond) if ( !(cond) ) { klib::kDebug::BreakPoint(); }
#else
#define WARN(cond) (void)0
#endif