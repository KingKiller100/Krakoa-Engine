#pragma once

#include <Utility/Logging/kLogProfile.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace krakoa
{
	using LogProfile = klib::LogProfileRef;
	template <typename T>
	using LogDest = klib::LogDispatcher::LogDestRef<T>;
}
