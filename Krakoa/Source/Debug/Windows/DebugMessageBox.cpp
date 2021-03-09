#include "Precompile.hpp"
#include "DebugMessageBox.hpp"

#if KRAKOA_OS_WINDOWS

#include <Utility/Enum/kEnum.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/Debug/Source/kSourceInfoToString.hpp>
#include <Utility/Debug/kDebugger.hpp>

// #include <iostream>

#include <Windows.h>

namespace krakoa::debug::win32
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

	void RaiseMessageBox_Win32(const std::string_view& title, const std::string_view& msg, const klib::SourceInfo& sourceInfo, long optionsMask) noexcept
	{
		const auto caption = klib::ToString("[Desc] {0}"
			"\n[File]: {1}"
			"\n[Line]: {2}"
			"\n[Func]: {3}"
			, msg
			, sourceInfo.file
			, sourceInfo.line
			, sourceInfo.func
		);

		const auto ret = MessageBoxA(nullptr
			, caption.data()
			, title.data()
			, optionsMask);
		
		const auto response = static_cast<MessageBoxResponse>(ret);
		
		// std::cout << response.ToString();

		if (response == MessageBoxResponse::MSGBOX_CANCEL
			|| response == MessageBoxResponse::MSGBOX_ABORT)
		{
			exit(1);
		}
		else if (response.Compare(MessageBoxResponse::MSGBOX_TRY_AGAIN)
			|| response == MessageBoxResponse::MSGBOX_RETRY)
		{
			klib::BreakPoint();
		}
	}
}

#endif

