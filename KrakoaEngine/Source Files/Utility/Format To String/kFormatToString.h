#pragma once

#include <cstdio>
#include <string>

namespace krakoa
{
	namespace kFormatToString
	{
		template<typename T, typename ...Ts>
		std::string FormatToString(const char* format, T&& arg, Ts&& ...argPack)
		{
			char buffer[1024];
			_sprintf_p(buffer, sizeof buffer, format, arg, std::forward<Ts>(argPack)...);
			return buffer;
		}
	}
}
