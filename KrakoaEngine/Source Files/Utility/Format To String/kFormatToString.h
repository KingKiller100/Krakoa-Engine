#pragma once

#include <cstdio>
#include <type_traits>
#include <string>

namespace util
{
	namespace kFormatToString
	{
		// Only designed for ascii and wide ascii (unicode) string
		template<class CharT, typename T, typename ...Ts>
		constexpr std::basic_string<CharT> FormatToString(const CharT* format, T&& arg, Ts&& ...argPack)
		{			
			CharT buffer[1024];

			if _CONSTEXPR_IF(std::is_same_v<CharT, char>)
				_sprintf_p(buffer, sizeof buffer, format, arg, argPack...);
			else if _CONSTEXPR_IF(std::is_same_v<CharT, wchar_t>)
				_swprintf_p(buffer, sizeof buffer, format, arg, argPack...);
			else
				return CharT('\0');

			return buffer;	
		}
	}
}
