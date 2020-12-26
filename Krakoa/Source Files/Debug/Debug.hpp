#pragma once

#include <Utility/Debug/kAssert.hpp>
#include <Utility/Debug/kDebugger.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>

namespace krakoa::debug
{
	void FatalError(const std::string& msg, const klib::SourceInfo& sourceInfo);
}

#define KRK_FATAL_COND(cond, msg) kAssertCB(cond, msg, krakoa::debug::FatalError)
#define KRK_FATAL(msg) KRK_FATAL_COND(false, msg)

#if defined(KRAKOA_DEBUG) || defined(KRAKOA_TEST)
#	define KRK_PAUSE_COND(cond) if ( !(cond) ) { klib::kDebug::BreakPoint(); }
#else
#	define KRK_PAUSE_COND(cond) ((void)0)
#endif