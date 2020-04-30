#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace kmaths::constants
{
	using AccuracyType = long double;

	constexpr AccuracyType E                         = 2.71828182845905l;
	constexpr AccuracyType LOG_N                     = 0.57721566490153l;
	constexpr AccuracyType LOG2E                     = 1.44269504088896l;
	constexpr AccuracyType LOG10E                    = 0.43429448190325l;
	constexpr AccuracyType LN2                       = 0.69314718055995l;
	constexpr AccuracyType LN10                      = 2.30258509299405l;
	constexpr AccuracyType PI                        = static_cast<AccuracyType>(M_PI);
	constexpr AccuracyType PI_OVER_2                 = PI / static_cast<AccuracyType>(2);//1.57079632679490l;
	constexpr AccuracyType PI_OVER_4                 = PI / static_cast<AccuracyType>(4); //0.78539816339745l;
	constexpr AccuracyType TAU                       = PI * static_cast<AccuracyType>(2);
	constexpr AccuracyType ROOT2                     = 1.41421356237310l;
	constexpr AccuracyType SQRT_1_OVER_2             = 0.70710678118655l;
	constexpr AccuracyType GOLDEN_RATIO              = 1.61803398874989l;
	constexpr AccuracyType INVERSE_GOLDEN_RATIO      = static_cast<AccuracyType>(1) / GOLDEN_RATIO;
}