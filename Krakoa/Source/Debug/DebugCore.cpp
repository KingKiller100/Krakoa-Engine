#include "Precompile.hpp"

#include "DebugCore.hpp"
#include "Windows/DebugMessageBox.hpp"

#include "../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>

#if KRAKOA_OS_WINDOWS
#	include <Windows.h>
#endif

namespace krakoa::debug
{
	void RaiseNotice(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		KRK_WRN(msg);

		const auto errorMsg = klib::kString::ToString("{0}"
			"\nClick \"Abort\" to close application."
			"\nClick \"Retry\" to break."
			"\nClick \"Ignore\" to continue."
			, msg);
		
#if KRAKOA_OS_WINDOWS
		win32::RaiseMessageBox_Win32("Krakoa Debug Error", errorMsg, sourceInfo, MB_ABORTRETRYIGNORE | MB_ICONERROR);
#endif
	}

	void RaiseException(const std::string& msg, const klib::SourceInfo& sourceInfo, klib::Logging& logger)
	{
		logger.AddFatal({
			msg
			, sourceInfo
			, klib::CalendarInfoSourceType::LOCAL
			});

		throw std::exception();
	}
}
