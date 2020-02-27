#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4996)

namespace klib
{
	namespace kFormat
	{
		template<class CharType, typename T>
		constexpr std::basic_string<CharType> ToString( T object )
		{
			std::basic_stringstream<CharType> ss;
			ss << object;
			return ss.str();
		}

		// Only designed for ANSI or wide char string
		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const CharType* format, T&& arg, Ts&& ...argPack)
		{
			size_t length = 0;
			std::unique_ptr<CharType[]> buffer;

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				length = _snprintf(nullptr, 0, format, arg, argPack...) + 1;
				if (!length) throw std::runtime_error("Error during \"ToString(...)\" formatting");
				buffer = std::unique_ptr<CharType[]>(new CharType[length]);
				sprintf_s(buffer.get(), length, format, arg, argPack...);
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				length = _snwprintf(nullptr, 0, format, arg, argPack...) + 1;
				if (!length) throw std::runtime_error("Error during \"ToString(...)\" formatting");
				buffer = std::unique_ptr<CharType[]>(new CharType[length]);
				swprintf_s(buffer.get(), length, format, arg, argPack...);
			}
			else
				return CharType('\0');
			
			const auto formattedText = std::basic_string<CharType>(buffer.get(), buffer.get() + length - 1);
			return formattedText;
		}
		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const std::basic_string<CharType>& format, T&& arg, Ts&& ...argPack)
		{
			const std::basic_string<CharType> text = ToString(format.c_str(), arg, argPack...);
			return text;
		}
	}
}

#	pragma warning(pop)
#endif