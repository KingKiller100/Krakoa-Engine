#include "Precompile.hpp"
#include "DebugWindows.hpp"

#include <iostream>

#include <Utility/Enum/kEnum.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/Debug/Source/kSourceInfoToString.hpp>

#ifdef KRAKOA_OS_WINDOWS

#include <Windows.h>

namespace krakoa::debug::windows
{
	ENUM_CLASS(MessageBoxResponse, long,
		MSG_CANCEL = 2,
		MSG_ABORT,
		MSG_RETRY,
		MSG_IGNORE,
		MSG_YES,
		MSG_NO
	);

	void RaiseNoticeImpl(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		const auto caption = klib::ToString("{0}\n Source: {1}", msg, sourceInfo);

		const MessageBoxResponse response = static_cast<MessageBoxResponse>(
			MessageBoxA(nullptr, caption.data(), "ERROR OCCURRED", MB_YESNOCANCEL)
			);
		const auto valueStr = response.ToString();
		std::cout << valueStr;
	}
}

#endif

