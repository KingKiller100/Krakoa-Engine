#pragma once
#include <functional>

#include <Utility/Enum/kEnum.hpp>

#ifdef ERROR
#	undef ERROR
#endif


namespace krakoa::os
{
	using MessageBoxOption_t = long;

	ENUM_CLASS( MessageBoxButton, MessageBoxOption_t,
		OKAY,
		OKAY_CANCEL,
		ABORTRE_TRY_IGNORE,
		YES_NO_CANCEL,
		YES_NO,
		RETRY_CANCEL,
		CANCEL_TRY_CONTINUE
	);

	ENUM_CLASS( MessageBoxIcon, MessageBoxOption_t,
		HAND,
		QUESTION,
		EXCLAMATION,
		ASTERISK,
		USER,
		WARNING,
		ERROR,
		INFORMATION,
		STOP 
	);

	ENUM_CLASS( MessageBoxDefaultButton, MessageBoxOption_t,
		BTN_1,
		BTN_2,
		BTN_3,
		BTN_4 
	);

	ENUM_CLASS(MessageBoxModal, MessageBoxOption_t,
		APPLY,
		SYSTEM,
		TASK,
		HELP
	);

	ENUM_CLASS(MessageBoxPosition, MessageBoxOption_t,
		NO_FOCUS,
		SET_FOREGROUND,
		DEFAULT_DESKTOP_ONLY,
		TOPMOST,
		RIGHT,
		RTLREADING);

	ENUM_CLASS( MessageBoxResponse, MessageBoxOption_t,
		MSGBOX_OKAY,
		MSGBOX_CANCEL,
		MSGBOX_ABORT,
		MSGBOX_RETRY,
		MSGBOX_IGNORE,
		MSGBOX_YES,
		MSGBOX_NO,
		MSGBOX_CLOSE,
		MSGBOX_HELP,
		MSGBOX_TIMEOUT,
		MSGBOX_TRY_AGAIN,
		MSGBOX_CONTINUE
	);

	using MessageBoxCallBack = void( MessageBoxResponse );

	class MessageBoxDisplay
	{
	public:
		static void Show(
			const std::string_view& title, const std::string_view& text, MessageBoxOption_t optionsMask
			, const std::function<MessageBoxCallBack>& cb
		);
	};
}
