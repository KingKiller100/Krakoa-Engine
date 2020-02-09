#pragma once

#include <string>

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
			CharType* buffer;

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				buffer = new CharType[1024]();
				_sprintf_p(buffer, 1024, format, arg, argPack...);
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				buffer = new CharType[2048]();
				_swprintf_p(buffer, 2048, format, arg, argPack...);
			}
			else
				return CharType('\0');

			return std::basic_string<CharType>(buffer);
		}
	}
}
