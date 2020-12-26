#include "Precompile.hpp"
#include "Debug.hpp"

#include "../Core/Logging/CoreLogger.hpp"

namespace krakoa::debug
{
	void FatalError(const std::string& msg, const klib::SourceInfo& sourceInfo, klib::Logging& logger)
	{
		logger.AddFatal({
			msg
			, sourceInfo
			, klib::CalendarInfoSourceType::LOCAL
			});
	}
}
