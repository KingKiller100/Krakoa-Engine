#pragma once

#include "../MessageBox/MessageBox.hpp"
#include <string>

namespace krakoa::os
{
	class MessageBoxDisplayWindows
	{
	public:
		static long Show( const std::string_view& title, const std::string_view& text, MessageBoxOption_t optionsMask );
	};
}
