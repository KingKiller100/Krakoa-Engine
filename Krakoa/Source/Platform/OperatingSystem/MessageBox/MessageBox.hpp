#pragma once
#include <functional>

#include <Utility/Enum/kEnum.hpp>

#ifdef ERROR
#	undef ERROR
#endif


namespace krakoa::os
{
	using MessageBoxOption_t = long;

	ENUM_CLASS( MessageBoxButtonSet, MessageBoxOption_t,
		OKAY,
		OKAY_CANCEL,
		ABORT_RETRY_IGNORE,
		YES_NO_CANCEL,
		YES_NO,
		RETRY_CANCEL,
		CANCEL_TRY_CONTINUE,
		HELP
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

	ENUM_CLASS( MessageBoxModality, MessageBoxOption_t,
		BLOCK_WINDOW_INPUT,
		BLOCK_ALL_WINDOWS_INPUT
	);

	ENUM_CLASS( MessageBoxPosition, MessageBoxOption_t,
		NO_FOCUS,
		SET_FOREGROUND,
		DEFAULT_DESKTOP_ONLY,
		TOPMOST,
		RIGHT,
		RTLREADING,
		SERVICE_NOTIFICATION
	);

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
			std::string_view title, std::string_view text
			, const std::function<MessageBoxCallBack>& cb
			, MessageBoxButtonSet buttons
			, MessageBoxIcon icon
			, MessageBoxDefaultButton defaultButton = MessageBoxDefaultButton::BTN_1
			, MessageBoxModality modality = MessageBoxModality::BLOCK_WINDOW_INPUT
			, MessageBoxPosition positioning = MessageBoxPosition::TOPMOST
		);
	};
}
