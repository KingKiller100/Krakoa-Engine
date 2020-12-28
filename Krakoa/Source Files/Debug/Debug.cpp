#include "Precompile.hpp"
#include "Debug.hpp"

#include "Windows/DebugWindows.hpp"

namespace krakoa::debug
{
	using namespace windows;
	
	void RaiseNotice(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		RaiseNoticeImpl(msg, sourceInfo);
	}

	void RaiseException(const std::string& msg, const klib::SourceInfo& sourceInfo, klib::Logging& logger)
	{
		logger.AddFatal({
			msg
			, sourceInfo
			, klib::CalendarInfoSourceType::LOCAL
			});
	}
}
