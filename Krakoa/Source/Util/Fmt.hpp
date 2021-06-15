#pragma once

#include <Utility/String/kToString.hpp>


namespace util
{
	template<class FormatT, typename T, typename ...Ts>
	USE_RESULT constexpr std::basic_string<FormatT> Fmt(const FormatT* format, const T& arg, const Ts& ...argPack)
	{
		return klib::ToString(format, arg, argPack...);
	}
}
