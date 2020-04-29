#pragma once

#include "../HelperMacros.hpp"

#include "Constants.hpp"

#include <cmath>
#include <utility>

#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

namespace kmaths
{
	template<typename DestType, typename SourceType>
	USE_RESULT constexpr DestType Convert(SourceType&& source)
	{
		if _CONSTEXPR_IF(std::is_arithmetic_v<DestType>)
		{
			constexpr auto max = std::numeric_limits<DestType>::max();
			constexpr auto min = std::numeric_limits<DestType>::min();

			if (source > max)
				return max;
			else if (source < min)
				return min;

			return DestType(source);
		}

		return DestType(std::forward<SourceType&&>(source));
	}

	template<typename T1, typename T2>
	USE_RESULT constexpr T1 Max(const T1& lhs, const T2& rhs) noexcept
	{
		return lhs > rhs ? lhs : (T1)rhs;
	}

	template<typename T1, typename T2>
	USE_RESULT constexpr T1 Min(const T1& lhs, const T2& rhs) noexcept
	{
		return lhs < rhs ? lhs : (T1)rhs;
	}

	template<typename T>
	USE_RESULT constexpr T PowerOf(T base, int power) noexcept(std::is_arithmetic_v<T>)
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, pow(base, power));
#else
		if (power == 0)
			return CAST(T, 1);
		else if (power == 1)
			return base;

		T value = base;

		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return CAST(T, 0);
		}
		else
		{
			if (power < 0)
			{
				base = CAST(T, 1) / base;
				value = base;
				power = -power;
			}
		}

		for (size_t i = 1; i < power; ++i)
			value *= base;

		return value;

#endif
	}

	template<typename T>
	USE_RESULT constexpr T Round(const T value, const unsigned char decimalPoints) noexcept
	{
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
			return value;

		const auto dpShifts = PowerOf<long double>(0.1, decimalPoints + 1) * 5;
		const auto accuracy = PowerOf<size_t>(10, decimalPoints);

		const auto valuePlusDpsByAcc = (value + dpShifts) * accuracy;
		const auto accuracyInverse = CAST(T, 1) / accuracy;
		const auto penultimateVal = CAST(long long, valuePlusDpsByAcc);
		const auto significantFigures = CAST(T, penultimateVal);
		const T roundedValue = significantFigures * accuracyInverse;
		return roundedValue;
	}

	template<typename T, class = std::enable_if_t<!std::is_rvalue_reference_v<T>>>
	constexpr void Swap(T& lhs, T& rhs) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>)
	{
		T temp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	template<typename T>
	USE_RESULT constexpr T ToDegrees(const T radians) noexcept
	{
		constexpr auto convertR2D = CAST(T, 360) / CAST(T, constants::TAU);
		return CAST(T, radians * convertR2D);
	}

	template<typename T>
	USE_RESULT constexpr T ToRadians(const T degrees) noexcept
	{
		constexpr auto convertR2D = CAST(T, 360) / CAST(T, constants::TAU);
		constexpr auto convertD2R = CAST(constants::AccuracyType, 1) / convertR2D;
		return CAST(T, degrees * convertD2R);
	}

	template<typename T>
	USE_RESULT constexpr T Clamp(const T value, const T min, const T max) noexcept
	{
		return (Min(max, Max(value, min)));
	}

	template<typename T>
	USE_RESULT constexpr T AbsClamp(const T value, const T min, const T max) noexcept
	{
		return value >= 0 ? Clamp(value, min, max) : -Clamp(value, min, max);
	}

	template<typename T>
	USE_RESULT constexpr T Remap(T progress, T actualMin, T actualMax, T remappedMin, T remappedMax) noexcept
	{
		const T actualDifference = actualMax - actualMin;
		const T remappedDifference = remappedMax - remappedMin;
		const T actualProgress = (progress - actualMin) / actualDifference;
		const T remappedProgress = remappedMin + remappedMax * actualProgress;

		return remappedProgress;
	}

	template<typename T>
	USE_RESULT constexpr T GetRange(const T minVal, const T maxVal) noexcept
	{
		return maxVal - minVal;
	}

	template <typename T>
	USE_RESULT constexpr T lerp(T a, T b, T t) noexcept
	{
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T lerpClampled(T a, T b, T t) noexcept
	{
		t = Clamp<T>(t, 0, 1);
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T lerpPartial(T a, T b, T t, T tmin, T tmax) noexcept
	{
		t = Clamp<T>(t, tmin, tmax);

		t -= tmin;
		t /= (tmax - tmin);

		return lerpClampled<T>(a, b, t);
	}

	template <typename T>
	USE_RESULT T getAccelerationOverTime(T initialVelocity, T distance) noexcept
	{
		const T acceleration = (0 - (initialVelocity * initialVelocity)) / (2 * distance);

		return acceleration;
	}

	template <typename T>
	USE_RESULT constexpr T getTimeTakenForAcceleration(T initialVelocity, T distance, T finalVelocity) noexcept
	{
		const T timeResult = (finalVelocity - initialVelocity) / getAccelerationOverTime(initialVelocity, distance);

		return timeResult;
	}

	template <typename T>
	USE_RESULT constexpr T lerpWithAcceleration(T initialVelocity, T currentTime, T distance) noexcept
	{
		const T acceleration = getAccelerationOverTime<T>(initialVelocity, distance);
		const T result = (initialVelocity * currentTime) + ((acceleration / 2) * (currentTime * currentTime));
		const T currentvel = initialVelocity + (acceleration * currentTime);
		return result;
	}

	template <typename T>
	USE_RESULT constexpr T modulus(T num, T base) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			if (num < 0.0f)
			{
				return fmod(num, base) + base;
			}
			return fmod(num, base);
		}

		T const rem = num % base;
		if _CONSTEXPR_IF(-1 % 2 == 1)
		{
			return rem;
		}

		return rem < 0 ? rem + base : rem;
	}
}


