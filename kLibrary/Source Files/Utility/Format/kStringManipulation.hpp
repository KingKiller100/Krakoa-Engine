#pragma once

#include "../../HelperMacros.hpp"

#include <string>

namespace klib::String
{
	template<class Char>
	USE_RESULT constexpr std::basic_string<Char> Replace(const std::basic_string_view<Char>& str, const Char oldChar, const Char newChar) noexcept
	{
		using String = std::basic_string<Char>;

		auto text = String(str);

		
		auto oldCharPos = str.find_first_of(oldChar);

		if (oldCharPos == String::npos)
			return text;

		if (str[0] == oldChar)
			text[0] = oldChar;

		do {
			text[oldCharPos] = newChar;
			oldCharPos = str.find_first_of(oldChar);
		} while (oldCharPos != String::npos);
		return text;
	}

	/*template<class Char>
	std::vector<std::basic_string<Char>> Split(const std::basic_string<Char>& str, const std::basic_string<Char> tokens, bool keepEmptyStr)
	{
		std::vec
	}*/


}


