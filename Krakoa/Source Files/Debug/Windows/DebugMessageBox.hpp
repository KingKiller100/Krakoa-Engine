#pragma once

#include <Utility/Debug/Source/kSourceInfo.hpp>

#include <string>

#if KRAKOA_OS_WINDOWS

namespace krakoa::debug::windows
{
	void RaiseMessageBox_Windows(const std::string_view& title, const std::string_view& msg, const klib::SourceInfo& sourceInfo, long optionsMask) noexcept;
}

#endif
