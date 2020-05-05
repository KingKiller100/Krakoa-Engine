#pragma once

#include "../HelperMacros.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <xtr1common>

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

	template<class T>
	USE_RESULT constexpr T Infinity() noexcept
	{
		return std::numeric_limits<T>::infinity();
	}

	template<class T>
	USE_RESULT constexpr T One() noexcept
	{
		return T(1);
	}

	template<class T>
	USE_RESULT constexpr T One(T&&) noexcept
	{
		return One<T>();
	}

	template<class T>
	USE_RESULT constexpr T MinusOne() noexcept
	{
		return T(-1);
	}

	template<class T>
	USE_RESULT constexpr T MinusOne(T&&) noexcept
	{
		return MinusOne<T>();
	}

	template<class T>
	USE_RESULT constexpr T ZeroPointOne() noexcept
	{
		return T(0.1);
	}

	template<class T>
	USE_RESULT constexpr T ZeroPointOne(T&&) noexcept
	{
		return ZeroPointOne<T>();
	}

	template<class T>
	USE_RESULT constexpr T ZeroPointFive() noexcept
	{
		return T(0.5);
	}

	template<class T>
	USE_RESULT constexpr T ZeroPointFive(T&&) noexcept
	{
		return ZeroPointFive<T>();
	}

	template<class T>
	USE_RESULT constexpr T OnePointFive() noexcept
	{
		return T(1.5);
	}

	template<class T>
	USE_RESULT constexpr T OnePointFive(T&&) noexcept
	{
		return OnePointFive<T>();
	}

	template<class ReturnType, class T>
	USE_RESULT constexpr ReturnType OneOver(T value) noexcept
	{
		return One<ReturnType>() / value;
	}

	template<class ReturnType, class T, class T2>
	USE_RESULT constexpr ReturnType XOverY(T&& x, T2&& y) noexcept
	{
		return ReturnType(std::forward<T&&>(x)) / y;
	}
}
