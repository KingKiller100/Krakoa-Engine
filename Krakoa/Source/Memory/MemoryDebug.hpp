#pragma once

#include "../Debug/DebugCore.hpp"
#include "../Core/EngineMacros.hpp"
#include "../Logging/MemoryLogger.hpp"
#include "../Util/Fmt.hpp"

#include <Utility/Debug/kAssert.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>


#if KRK_ENABLE_ASSERT

#	define MEM_ASSERT(cond, msg) if (!(cond)) throw klib::FailedConditionException(#cond, msg, SOURCE_INFO(),\
[](std::string_view e, std::string_view m, const klib::SourceInfo& s)\
{\
	const auto err = ::util::Fmt("[FAILED] {0} Condition: \"{1}\" ", e, m);\
	::krakoa::debug::RaiseNotice(err, s);\
})\

#else
#	define MEM_ASSERT(cond, msg) ((void)0)
#endif

#define MEM_FATAL(msg) ::krakoa::debug::RaiseException(msg, SOURCE_INFO())

