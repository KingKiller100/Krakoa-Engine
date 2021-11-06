#include "Precompile.hpp"

#include "MessageBoxWindows.hpp"

#include <Windows.h>

#ifdef ERROR
#	undef ERROR
#endif


namespace krakoa::os
{
	namespace 
	{
	}

	long MessageBoxDisplayWindows::Show(
		const std::string_view& title, const std::string_view& text, MessageBoxOption_t optionsMask
	)
	{
		const auto response = ::MessageBoxA( nullptr
			, text.data()
			, title.data()
			, optionsMask );

		return ResolveResponse( response );
	}

	MessageBoxResponse MessageBoxDisplayWindows::ResolveResponse( long response )
	{
		static std::unordered_map<MessageBoxResponse::Underlying_t, MessageBoxResponse::Value> responsesMap =
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

	MessageBoxIcon MessageBoxDisplayWindows::ResolveIcon( long icon )
	{
		static std::unordered_map<MessageBoxIcon::Underlying_t, MessageBoxIcon::Value> iconMap =
		{
			{MB_ICONHAND, MessageBoxIcon::HAND},
			{MB_ICONQUESTION, MessageBoxIcon::QUESTION},
			{MB_ICONEXCLAMATION, MessageBoxIcon::EXCLAMATION},
			{MB_ICONASTERISK, MessageBoxIcon::ASTERISK},
			{MB_USERICON, MessageBoxIcon::USER},
			{MB_ICONWARNING, MessageBoxIcon::WARNING},
			{MB_ICONERROR, MessageBoxIcon::ERROR},
			{MB_ICONINFORMATION, MessageBoxIcon::INFORMATION},
			{MB_ICONSTOP, MessageBoxIcon::STOP},
		};

		return iconMap.at( icon );
	}

	MessageBoxDefaultButton MessageBoxDisplayWindows::ResolveDefaultButton( long defBtn )
	{
		static std::unordered_map<MessageBoxDefaultButton::Underlying_t, MessageBoxDefaultButton::Value> defaultBtnMap =
		{
		{MB_DEFBUTTON1, MessageBoxDefaultButton::BTN_1},
		{MB_DEFBUTTON2, MessageBoxDefaultButton::BTN_2},
		{MB_DEFBUTTON3, MessageBoxDefaultButton::BTN_3},
		{MB_DEFBUTTON4, MessageBoxDefaultButton::BTN_4}
		};

		return defaultBtnMap.at( defBtn );
	}
}
