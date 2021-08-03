#pragma once

#include <Utility/Logging/kLogging.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>
#include <Utility/Debug/Exceptions/NotImplementedException.hpp>


namespace krakoa::debug
{
	void RaiseNotice(const std::string_view& msg, const klib::SourceInfo& sourceInfo);
	void RaiseException(const std::string_view& msg, const klib::SourceInfo& sourceInfo, klib::Logging& logger);
}