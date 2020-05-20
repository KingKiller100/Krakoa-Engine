#pragma once

#include "../HelperMacros.hpp"

#include <limits>
#include <type_traits>

namespace kmaths
{
	using Length_Type = unsigned short;

	using Big_Int_Type = std::make_signed_t<size_t>;

	template<typename T>
	constexpr auto Max_Decimal_Precision_V = std::numeric_limits<T>::max_digits10;
}