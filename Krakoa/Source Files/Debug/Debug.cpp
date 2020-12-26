#include "Precompile.hpp"
#include "Debug.hpp"

#include "../Core/Logging/CoreLogger.hpp"

#include <Utility/Debug/kAssert.hpp>

namespace krakoa::debug
{
	void FatalError(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		CoreLogger::GetCoreLogger().OutputToFatalFile({
			msg
			, sourceInfo
			, klib::CalendarInfoSourceType::LOCAL
		});

		throw std::exception();
	}
}
