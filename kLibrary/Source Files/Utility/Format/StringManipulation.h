#pragma once

#include "HelperMacros.hpp"

#include <string>

namespace klib::String
{
	template<class Char>
	void Replace(std::basic_string<Char>& str, const Char oldChars, const Char newChars)
	{
		if (str[0] == oldChars)
			str[0] = oldChars;
		
		auto oldCharPos = str.find_first_of(oldChars);

		do
		{
			const auto start = str.substr(0, oldCharPos);
			const auto end = str.substr(oldCharPos + 1);
			str = start + oldChars + end;
		} while (oldCharPos != 0);
	}

	template<class Char>
	std::basic_string<Char> Replace(const std::basic_string<Char>& str, const Char oldChars, const Char newChars)
	{
		auto text = str;
		Replace(text, oldChars, newChars);
		return text;
	}

	/*template<class Char>
	std::vector<std::basic_string<Char>> Split(const std::basic_string<Char>& str, const std::basic_string<Char> tokens, bool keepEmptyStr)
	{
		std::vec
	}*/
	
	
}


