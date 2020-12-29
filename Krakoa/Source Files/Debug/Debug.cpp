#include "Precompile.hpp"
#include "Debug.hpp"

#include "Windows/DebugMessageBox.hpp"

#include <Utility/String/kToString.hpp>

#if KRAKOA_OS_WINDOWS
#	include <Windows.h>
#endif

namespace krakoa::debug
{
	void RaiseNotice(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		const auto errorMsg = klib::kString::ToString("{0}\n"
			"Click \"OK\" to continue.\n"
			"Click \"Cancel\" to close application.\n"
			, msg);

#if KRAKOA_OS_WINDOWS
		windows::RaiseMessageBox_Windows("Krakoa Debug Error", errorMsg, sourceInfo, MB_OKCANCEL | MB_ICONERROR);
#endif
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
