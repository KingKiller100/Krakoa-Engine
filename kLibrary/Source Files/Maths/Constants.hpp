#pragma once

#include <cmath>

namespace kmaths::consts
{
	using AccuracyType = long double;

	constexpr AccuracyType E = 2.718281828459045235360287;
	constexpr AccuracyType LOG2E = 1.44269504088896340736;
	constexpr AccuracyType LOG10E = 0.434294481903251827651;
	constexpr AccuracyType LN2 = 0.693147180559945309417;
	constexpr AccuracyType LN10 = 2.30258509299404568402;
	constexpr AccuracyType PI = 3.141592653589793238462643;
	constexpr AccuracyType PI_DIV_2 = 1.57079632679489661923;
	constexpr AccuracyType PI_DIV_4 = 0.785398163397448309616;
	constexpr AccuracyType TAU = 2 * PI;
	constexpr AccuracyType LOG_N = 0.577215664901532860606512;
	constexpr AccuracyType ROOT2 = 1.414213562373095048801688;
	constexpr AccuracyType SQRT1_2 = 0.707106781186547524401;
	constexpr AccuracyType GOLDEN_RATIO = 1.618033988749894848204586;
	constexpr AccuracyType INVERSE_GOLDEN_RATIO = static_cast<AccuracyType>(1) / GOLDEN_RATIO;
}