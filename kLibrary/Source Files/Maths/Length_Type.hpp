#pragma once

#include "../HelperMacros.hpp"

namespace kmaths
{
	using Length_Type = unsigned short;

#ifdef _WIN64
	using Big_Int_Type = long long;
#else
	using Big_Int_Type = int;
#endif

	template<typename T>
	constexpr auto Max_DP_Precision = std::numeric_limits<T>::max_digits10;
}