#pragma once

#include <cmath>

namespace util
{
	namespace kMaths
	{
		using AccuracyType = long double;

		constexpr AccuracyType E = 2.718281828459045235360287;
		constexpr AccuracyType PI = 3.141592653589793238462643;
		constexpr AccuracyType TAU = 2 * PI;
		constexpr AccuracyType LOG_N = 0.577215664901532860606512;
		constexpr AccuracyType ROOT_2 = 1.414213562373095048801688;
		constexpr AccuracyType GOLDEN_RATIO = 1.618033988749894848204586;
		constexpr AccuracyType INVERSE_GOLDEN_RATIO = 1 / GOLDEN_RATIO;
		
		template<typename T1, typename T2>
		T1 PowerOfX(T1&& base, T2&& power)
		{
			return pow(base, power);
		}		
	}
}