#pragma once

#include "../../HelperMacros.hpp"

#include <string>
#include <vector>

namespace klib::kString
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL basic_string_view
	template<class Char>
	using StringReader = std::basic_string_view<ONLY_TYPE(Char)>;

	// STL basic_string
	template<class Char>
	using StringWriter = std::basic_string<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>, std::allocator<ONLY_TYPE(Char)>>;

	// --------------------------------------------------------------------------------------

	template<class Char = char>
	USE_RESULT constexpr StringWriter<Char> Replace(const StringReader<Char>& str, const ONLY_TYPE(Char) oldChar, const ONLY_TYPE(Char) newChar) noexcept
	{
		using StrW = StringWriter<Char>;

		auto text = StrW(str);

		auto oldCharPos = str.find_first_of(oldChar);

		if (oldCharPos == StrW::npos)
			return text;

		if (str[0] == oldChar)
			text[0] = oldChar;

		do {
			text[oldCharPos] = newChar;
			oldCharPos = text.find_first_of(oldChar);
		} while (oldCharPos != StrW::npos);
		return text;
	}

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const StringReader<Char>& tokens, const bool preserveToken = false)
	{
		using StrW = StringWriter<Char>;

		std::vector<StrW> lines;

		size_t prevPos = 0;
		size_t tokenPos = 0;
		while (tokenPos != StrW::npos)
		{
			tokenPos = str.find_first_of(tokens.data(), prevPos);
			lines.emplace_back(str.substr(prevPos, (preserveToken ? tokenPos + 1 : tokenPos) - prevPos));
			prevPos = tokenPos + 1;
		}

		return lines;
	}


}


