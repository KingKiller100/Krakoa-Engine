#pragma once

#include "kStringTypes.hpp"

#include <algorithm>
#include <vector>

namespace klib::kString
{
	enum class PreserveToken { YES, NO, };
	enum class PreserveEmpty { YES, NO, };
	
	template<class Char = char>
	USE_RESULT constexpr StringWriter<Char> Replace(const StringReader<Char>& str, const ONLY_TYPE(Char) oldChar, const ONLY_TYPE(Char) newChar) noexcept
	{
		using StrW = StringWriter<Char>;

		auto text = StrW(str);

#if MSVC_PLATFORM_TOOLSET >= 141
		std::replace(text.begin(), text.end(), oldChar, newChar);
#else
		auto oldCharPos = str.find_first_of(oldChar);

		if (oldCharPos == StrW::npos)
			return text;

		if (str[0] == oldChar)
			text[0] = oldChar;

		do {
			text[oldCharPos] = newChar;
			oldCharPos = text.find_first_of(oldChar);
		} while (oldCharPos != StrW::npos);
#endif
		return text;
	}

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const Char* tokens, const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
	{
		using StrW = StringWriter<Char>;

		const auto keepToken = (preserveToken == PreserveToken::YES);
		const auto keepEmpty = (preserveEmpty == PreserveEmpty::YES);
		
		std::vector<StrW> lines;

		size_t prevPos = 0;
		size_t tokenPos = 0;
		while (tokenPos != StrW::npos)
		{
			tokenPos = str.find_first_of(tokens, prevPos);
			const auto line = str.substr(prevPos, (keepToken ? tokenPos + 1 : tokenPos) - prevPos);
			if (!line.empty() || keepEmpty)
				lines.emplace_back(line);
			prevPos = tokenPos + 1;
		}

		return lines;
	}

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const StringReader<Char>& tokens, const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
	{
		return Split(str, tokens.data(), preserveToken, preserveEmpty);
	}

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const ONLY_TYPE(Char)& token, const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
	{
		const auto*  ptr = &token;
		return Split(str, ptr, preserveToken, preserveEmpty);
	}

	template<class Char = char>
	USE_RESULT constexpr Char* ToUpper(const Char* input)
	{
		using StrR = StringReader<Char> ;
		StrR in(input);
		Char* output = new Char[in.length() + 1]{};
		
		for (auto i = 0; i < in.length(); ++i)
		{
			Char c = in[i];
			if (c >= Char('a') && c <= Char('z'))
				output[i] = CAST(Char, c - 32);
			else
				output[i] = c;
		}

		return output;
	}

	template<class Char = char>
	USE_RESULT constexpr Char* ToLower(const Char* input)
	{
		using StrR = StringReader<Char> ;
		StrR in(input);
		Char* output = new Char[in.length() + 1]{};
		
		for (auto i = 0; i < in.length(); ++i)
		{
			Char c = in[i];
			if (c >= Char('A') && c <= Char('Z'))
				output[i] = CAST(Char, c + 32);
			else
				output[i] = c;
		}

		return output;
	}

	template<class T, class = std::enable_if_t<type_trait::Is_StringType_V< T >>>
	USE_RESULT constexpr StringWriter<typename T::value_type > ToUpper(const T& input)
	{
		return ToUpper(input.data());
	}

	template<class T, class = std::enable_if_t<type_trait::Is_StringType_V< T >>>
	USE_RESULT constexpr StringWriter<typename T::value_type > ToLower(const T& input)
	{
		return ToLower<typename T::value_type>(input.data());
	}
}


