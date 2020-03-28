#pragma once

#include <cmath>

namespace kmaths::constants
{
	using AccuracyType = long double;

	constexpr AccuracyType E                         = 2.71828182845905;
	constexpr AccuracyType LOG2E                     = 1.44269504088896;
	constexpr AccuracyType LOG10E                    = 0.43429448190325;
	constexpr AccuracyType LN2                       = 0.69314718055995;
	constexpr AccuracyType LN10                      = 2.30258509299405;
	constexpr AccuracyType PI                        = 3.14159265358979;
	constexpr AccuracyType PI_DIV_2                  = 1.57079632679490;
	constexpr AccuracyType PI_DIV_4                  = 0.78539816339745;
	constexpr AccuracyType TAU                       = PI * static_cast<AccuracyType>(2);
	constexpr AccuracyType LOG_N                     = 0.57721566490153;
	constexpr AccuracyType ROOT2                     = 1.41421356237310;
	constexpr AccuracyType SQRT1_2                   = 0.70710678118655;
	constexpr AccuracyType GOLDEN_RATIO              = 1.61803398874989;
	constexpr AccuracyType INVERSE_GOLDEN_RATIO      = static_cast<AccuracyType>(1) / GOLDEN_RATIO;
}