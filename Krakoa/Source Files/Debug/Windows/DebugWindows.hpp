#pragma once

#include <Utility/Debug/Source/kSourceInfo.hpp>

#include <string>

#if KRAKOA_OS_WINDOWS

namespace krakoa::debug::windows
{
	void RaiseNoticeImpl(const std::string_view& msg, const klib::SourceInfo& sourceInfo);
}

#endif
