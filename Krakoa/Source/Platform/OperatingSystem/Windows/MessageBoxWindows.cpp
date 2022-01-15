#include "Precompile.hpp"

#include "MessageBoxWindows.hpp"

#include <Windows.h>

#ifdef ERROR
#	undef ERROR
#endif


namespace krakoa::os
{
	long MessageBoxDisplayWindows::Show(
		std::string_view title, std::string_view text
		, MessageBoxButtonSet buttons
		, MessageBoxIcon icon
		, MessageBoxDefaultButton defaultButton
		, MessageBoxModality modality
		, MessageBoxPosition positioning
	)
	{
		const auto btnSet = ResolveButttonSet( buttons );
		const auto ic = ResolveIcon( icon );
		const auto defBtn = ResolveDefaultButton( defaultButton );
		const auto modal = ResolveModal( modality );
		const auto pos = ResolvePositioning( positioning );

		const auto flags = btnSet | ic | defBtn | modal | pos;

		const auto response = ::MessageBoxA( nullptr
			, text.data()
			, title.data()
			, flags );

		return ResolveResponse( response );
	}

	MessageBoxResponse MessageBoxDisplayWindows::ResolveResponse( long response )
	{
		static const std::unordered_map<MessageBoxResponse::Underlying_t, MessageBoxResponse::Value> responsesMap =
		{
		{IDOK      , MessageBoxResponse::MSGBOX_OKAY       },
		{IDCANCEL  , MessageBoxResponse::MSGBOX_CANCEL     },
		{IDABORT   , MessageBoxResponse::MSGBOX_ABORT      },
		{IDRETRY   , MessageBoxResponse::MSGBOX_RETRY      },
		{IDIGNORE  , MessageBoxResponse::MSGBOX_IGNORE     },
		{IDYES     , MessageBoxResponse::MSGBOX_YES        },
		{IDNO      , MessageBoxResponse::MSGBOX_NO         },
		{IDCLOSE   , MessageBoxResponse::MSGBOX_CLOSE      },
		{IDHELP    , MessageBoxResponse::MSGBOX_HELP       },
		{IDTRYAGAIN, MessageBoxResponse::MSGBOX_TRY_AGAIN  },
		{IDCONTINUE, MessageBoxResponse::MSGBOX_CONTINUE   },
		{IDTIMEOUT , MessageBoxResponse::MSGBOX_TIMEOUT    }
		};

		return responsesMap.at(response);
	}

	long MessageBoxDisplayWindows::ResolveButttonSet( MessageBoxButtonSet btnSet )
	{
		static const std::unordered_map < MessageBoxButtonSet::Value, long > btnSets_win32 =
		{
			{MessageBoxButtonSet::OKAY, MB_OK},
			{MessageBoxButtonSet::OKAY_CANCEL, MB_OKCANCEL},
			{MessageBoxButtonSet::ABORT_RETRY_IGNORE, MB_ABORTRETRYIGNORE},
			{MessageBoxButtonSet::YES_NO_CANCEL, MB_YESNOCANCEL},
			{MessageBoxButtonSet::YES_NO, MB_YESNO},
			{MessageBoxButtonSet::RETRY_CANCEL, MB_RETRYCANCEL},
			{MessageBoxButtonSet::CANCEL_TRY_CONTINUE, MB_CANCELTRYCONTINUE},
			{MessageBoxButtonSet::HELP, MB_HELP}
		};

		return btnSets_win32.at(btnSet);
	}

	long MessageBoxDisplayWindows::ResolveIcon( MessageBoxIcon icon )
	{
		static const std::unordered_map < MessageBoxIcon::Value, long > icons_win32 =
		{
		{MessageBoxIcon::ASTERISK, MB_ICONASTERISK},
		{MessageBoxIcon::HAND, MB_ICONHAND},
		{MessageBoxIcon::ERROR, MB_ICONERROR},
		{MessageBoxIcon::EXCLAMATION, MB_ICONEXCLAMATION},
		{MessageBoxIcon::QUESTION, MB_ICONQUESTION},
		{MessageBoxIcon::USER, MB_USERICON},
		{MessageBoxIcon::WARNING, MB_ICONWARNING},
		{MessageBoxIcon::INFORMATION, MB_ICONINFORMATION},
		{MessageBoxIcon::STOP, MB_ICONSTOP},
		};

		return icons_win32.at( icon );
	}

	long MessageBoxDisplayWindows::ResolveDefaultButton( MessageBoxDefaultButton defBtn )
	{
		static const std::unordered_map < MessageBoxDefaultButton::Value, long > defBtns_win32 =
		{
			{MessageBoxDefaultButton::BTN_1, MB_DEFBUTTON1},
			{MessageBoxDefaultButton::BTN_2, MB_DEFBUTTON2},
			{MessageBoxDefaultButton::BTN_3, MB_DEFBUTTON3},
			{MessageBoxDefaultButton::BTN_4, MB_DEFBUTTON4},
		};

		return defBtns_win32.at( defBtn );
	}

	long MessageBoxDisplayWindows::ResolveModal( MessageBoxModality modal )
	{
		static const std::unordered_map < MessageBoxModality::Value, long > modals_win32 =
		{
			{MessageBoxModality::BLOCK_WINDOW_INPUT, MB_APPLMODAL},
			{MessageBoxModality::BLOCK_ALL_WINDOWS_INPUT, MB_TASKMODAL},
		};

		return modals_win32.at( modal );
	}

	long MessageBoxDisplayWindows::ResolvePositioning( MessageBoxPosition positioning )
	{
		static const std::unordered_map < MessageBoxPosition::Value, long > positionings_win32 =
		{
			{MessageBoxPosition::RIGHT, MB_RIGHT},
			{MessageBoxPosition::TOPMOST, MB_TOPMOST},
			{MessageBoxPosition::NO_FOCUS, MB_NOFOCUS},
			{MessageBoxPosition::RTLREADING, MB_RTLREADING},
			{MessageBoxPosition::SET_FOREGROUND, MB_SETFOREGROUND},
			{MessageBoxPosition::DEFAULT_DESKTOP_ONLY, MB_DEFAULT_DESKTOP_ONLY},
			{MessageBoxPosition::SERVICE_NOTIFICATION, MB_SERVICE_NOTIFICATION},
		};

		return positionings_win32.at( positioning );
	}
}
