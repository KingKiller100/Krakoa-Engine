#pragma once

#include "../HelperMacros.hpp"

#include "Constants.hpp"

#include <cmath>

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
	USE_RESULT constexpr T PowerOf(T base, T power) noexcept(std::is_arithmetic_v<T>)
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, pow(base, power));
#else
		T value = 1;

		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power >= 1)
			{
				value = base;
				for (auto i = 1u; i < power; ++i)
					value *= base;
			}
			else if (power == 1)
			{
				return base;
			}
			else
			{
				value = 0;
			}
		}
		else
		{
			if (int(power) == power)
			{
				if (power == 0)
					return value;

				if (power == 1)
					return base;

				if (power >= 1)
					value = base;
				else
					value = CAST(T, 1) / base;

				auto pow = power;

				if (power < 0)
				{
					for (auto i = 0; i < 2; ++i)
						pow -= power;
				}

				const auto val = value;

				for (auto i = 1u; i < pow; ++i)
					value *= val;
			}
		}

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
		const T roundedValue = CAST(T, CAST(long long, valuePlusDpsByAcc)) / accuracy;
		return roundedValue;
	}

	template<typename T>
	USE_RESULT constexpr T ToDegrees(const T radians) noexcept
	{
		constexpr T convertR2D = CAST(T, 360) / constants::TAU;
		return radians * convertR2D;
	}

	template<typename T>
	USE_RESULT constexpr T ToRadians(const T degrees) noexcept
	{
		constexpr auto convertD2R = CAST(T, constants::TAU) / 360;
		return degrees * convertD2R;
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


