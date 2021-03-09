#pragma once

#include <Utility/Debug/Source/kSourceInfo.hpp>

#include <string>

#if KRAKOA_OS_WINDOWS

namespace krakoa::debug::win32
{
	void RaiseMessageBox_Win32(const std::string_view& title, const std::string_view& msg, const klib::SourceInfo& sourceInfo, long optionsMask) noexcept;
}

#endif
