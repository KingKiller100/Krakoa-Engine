#include "MessageBox.hpp"

#include "../Windows/MessageBoxWindows.hpp"

namespace krakoa::os
{
	void MessageBoxDisplay::Show(
		const std::string_view& title, const std::string_view& text, MessageBoxOption_t optionsMask
		, const std::function<MessageBoxCallBack>& cb
	)
	{
		const auto response = MessageBoxDisplayWindows::Show(title, text, optionsMask);

		if (cb)
			cb(response);
	}
}
