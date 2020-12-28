#include "Precompile.hpp"
#include "DebugWindows.hpp"

#include <iostream>

#include <Utility/Enum/kEnum.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/Debug/Source/kSourceInfoToString.hpp>

#if KRAKOA_OS_WINDOWS

#include <Windows.h>

namespace krakoa::debug::windows
{
	ENUM_CLASS(MessageBoxResponse, long,
		MSGBOX_OKAY = 1,
		MSGBOX_CANCEL,
		MSGBOX_ABORT,
		MSGBOX_RETRY,
		MSGBOX_IGNORE,
		MSGBOX_YES,
		MSGBOX_NO,
		MSGBOX_TRY_AGAIN = 10,
		MSGBOX_CONTINUE
	);

	void RaiseNoticeImpl(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		const auto caption = klib::ToString("Description: {0}\n"
			"Click \"OK\" to continue.\n"
			"Click \"Cancel\" to close application.\n"
			"[File]: {1} [{2}]\n"
			"[Function]: {3}\n"
			, msg
			, sourceInfo.file
			, sourceInfo.line
			, sourceInfo.func
		);

		const auto ret = MessageBoxA(nullptr
			, caption.data()
			, "Krakoa Debug Error"
			, MB_OKCANCEL | MB_ICONERROR);
		
		const auto response = static_cast<MessageBoxResponse>(ret);
		
		std::cout << response.ToString();

		if (response == MessageBoxResponse::MSGBOX_CANCEL)
			exit(1);
	}
}

#endif

