#pragma once

#include "../../Precompile.h"

namespace util
{
	namespace kFormatToString
	{
		template<class CharT, typename T>
		constexpr std::basic_string<CharT> SingleObjectToString( T object )
		{
			std::basic_stringstream<CharT> ss;
			ss << object;
			return ss.str();
		}

		// Only designed for ANSI or wide char string
		template<class CharT, typename T, typename ...Ts>
		constexpr std::basic_string<CharT> FormatToString( const CharT* format, T&& arg, Ts&& ...argPack )
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
