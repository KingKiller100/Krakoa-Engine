#include "Precompile.hpp"

#include "MessageBoxWindows.hpp"

#include <Windows.h>

namespace krakoa::os
{
	namespace 
	{
	}

	long MessageBoxDisplayWindows::Show(
		const std::string_view& title, const std::string_view& text, MessageBoxOption_t optionsMask
	)
	{
		static std::unordered_map<MessageBoxResponse::Underlying_t, MessageBoxResponse::Value> responsesWin32 =
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

		const auto response = ::MessageBoxA( nullptr
			, text.data()
			, title.data()
			, optionsMask );

		return responsesWin32.at( response );
	}
}
