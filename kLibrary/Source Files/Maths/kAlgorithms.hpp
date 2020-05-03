#pragma once

#include "../HelperMacros.hpp"

#include "Constants.hpp"
#include "Fraction.hpp"

#if MSVC_PLATFORM_TOOLSET > 142
#	include <cmath>
#endif
#include <cstdint>
#include <xtr1common>

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
		else
		{
			return DestType(std::forward<SourceType&&>(source));
		}
	}

	template<typename List, typename T>
	USE_RESULT constexpr long long BinarySearchImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
	{
		if (lbIdx > ubIdx
			|| value < list[0]
			|| value > list[size - 1])
			return -1;

		const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);
		const auto mid = list[midIdx];

		if (mid > value)
			return BinarySearchImpl(list, value, lbIdx, midIdx - 1, size);
		else if (mid < value)
			return BinarySearchImpl(list, value, midIdx + 1, ubIdx, size);

		return midIdx;
	}

	template<typename T>
	USE_RESULT constexpr long long BinarySearch(T* list, T&& value, size_t size) noexcept
	{
		return BinarySearchImpl(list, value, 0, size - 1, size);
	}

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr long long BinarySearch(const T(&list)[N], T&& value) noexcept
	{
		return BinarySearchImpl(list, value, 0, N - 1, N);
	}

	template<typename List, typename T>
	USE_RESULT constexpr long long BinarySearchClosestImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
	{
		if (lbIdx > ubIdx
			|| value < list[0]
			|| value > list[size - 1])
			return -1;

		const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);

		const auto lowerbound = list[midIdx];
		const auto upperbound = list[midIdx + 1];

		if (lowerbound == value)
			return midIdx;
		else if (value < lowerbound)
			return BinarySearchClosestImpl(list, value, lbIdx, midIdx - 1, size);
		else if (value > upperbound)
			return BinarySearchClosestImpl(list, value, midIdx + 1, ubIdx, size);

		const auto lbDiff = value - lowerbound;
		const auto ubDiff = upperbound - value;

		return lbDiff < ubDiff ? midIdx : midIdx + 1;
	}

	template< typename T>
	USE_RESULT constexpr long long BinarySearchClosest(const T* list, T&& value, size_t size)
	{
		return BinarySearchClosestImpl(list, value, 0, size - 1, size);
	};

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr long long BinarySearchClosest(const T(&list)[N], T&& value)
	{
		return BinarySearchClosestImpl(list, value, 0, N - 1, N);
	};

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsInteger(T value) noexcept
	{
		return Convert<int>(value) == value;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsDecimal(T value) noexcept
	{
		constexpr auto one = constants::One<T>();
		constexpr auto minusOne = constants::MinusOne<T>();

		const auto isNegative = value < 0;
		return isNegative
			? (value > minusOne&& value < 0)
			: (value < one && value > 0);
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

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Floor(const T value) noexcept
	{
		using ConversionType = long long;

		constexpr auto maxLL = std::numeric_limits<ConversionType>::max();
		constexpr auto minLL = std::numeric_limits<ConversionType>::min();

		if (value > maxLL || value < minLL)
			return value;

		const auto integer = CAST(T, CAST(ConversionType, value));

		return integer > value ? integer - 1 : integer;
			
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr Fraction DecimalToFraction(T x, const size_t dpAccuracy = 10) noexcept
	{
		constexpr size_t maxIterations = 10000;
		const auto error = PowerOfImpl(constants::ZeroPointOne<T>(), dpAccuracy);

		const auto isNegative = x < 0;

		if (isNegative)
			x = -x;

		const auto integer = Floor<T>(x);
		x -= integer;

		const auto oneMinusError = constants::One<T>() - error;
		const auto realInteger = Convert<Fraction::Numerator_Value_Type>(integer);

		if (x < error)
			return { realInteger, 1, isNegative };
		else if (oneMinusError < x)
			return { realInteger + 1, 1, isNegative };

		size_t lower_n = 0;
		size_t lower_d = 1;

		size_t upper_n = 1;
		size_t upper_d = 1;

		size_t mid_n;
		size_t mid_d;

		auto found = false;
		size_t iter = 0;

		do {
			mid_n = lower_n + upper_n;
			mid_d = lower_d + upper_d;

			const auto tooHigh = mid_n > mid_d* (x + error);
			const auto tooLow = mid_n < (x - error) * mid_d;

			if (tooHigh)
			{
				upper_n = mid_n;
				upper_d = mid_d;
			}
			else if (tooLow)
			{
				lower_n = mid_n;
				lower_d = mid_d;
			}

			found = !(tooHigh || tooLow);
		} while (iter++ < maxIterations && !found); // Binary search towards fraction

		return { realInteger * mid_d + mid_n, mid_d, isNegative };
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T PowerOfImpl(T base, long long power) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, pow(base, power));
#else
		if (power == 0)
			return constants::One<T>();
		else if (power == 1)
			return base;

		T value = base;

		if (power < 0)
		{
			if _CONSTEXPR_IF(std::is_arithmetic_v<T> && !std::is_floating_point_v<T>)
			{
				return 0;
			}
			else
			{
				base = constants::OneOver<T>(base);
				value = base;
				power = -power;
			}
		}

		while (--power > 0)
			value *= base;

		return value;
#endif
	}

	template<typename T>
	USE_RESULT constexpr decltype(auto) PowerOf10(long long power) noexcept(std::is_arithmetic_v<T>)
	{
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return 0;
		}
		else
		{
			return PowerOfImpl<T>(10, power);
		}
	}

	//USE_RESULT double Log(int base, double exponent)
	//{
	//	throw klib::kDebug::kExceptions::NotImplementedException("Log base 10 function not currently supported");

	//	double value = 1;
	//	int loops = 0;
	//	double increment = 0.1;
	//	double multiplier = base;
	//	double prev = -1;
	//	double ans = 0;

	//	while (prev != value)
	//	{
	//		prev = value;
	//		while (value < exponent)
	//		{
	//			value = 1;
	//			loops++;
	//			for (auto i = 0; i < loops; ++i)
	//			{
	//				value *= multiplier;
	//			}
	//		}
	//		ans = loops;
	//		loops = 0;
	//		multiplier = base + increment;
	//		increment *= 0.1;
	//	}

	//	return ans;
	//}

	USE_RESULT constexpr long long WhatPowerOf10(double number) noexcept
	{
		if (number < 1 && number > 0.1)
			return -1;
		if (number >= 1 && number < 10)
			return 0;
		if (number >= 10 && number < 100)
			return 1;

		if (number < 0)
			number = -number;

		long long currentPower = 1;
		if (number > 10)
		{
			long long value = 1;
			while (value < number)
			{
				currentPower++;
				value = PowerOfImpl(10, currentPower);
			}
			--currentPower;
		}
		else if (number < 1)
		{
			long double value = 0.1;
			while (value > number)
			{
				currentPower++;
				value = PowerOfImpl(0.1, currentPower);
			}
			currentPower = -currentPower;
		}

		return currentPower;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Round(T value, const uint8_t decimalPoints) noexcept
	{
		const auto isNegative = value < 0;
		if (isNegative)
			value = -value;

		const auto dpShifts = PowerOfImpl<long double>(constants::ZeroPointOne<T>(), long long(decimalPoints + 1)) * 5;
		const auto accuracy = PowerOfImpl<size_t>(10, decimalPoints);

		const auto valuePlusDpsByAcc = (value + dpShifts) * accuracy;
		const auto accuracyInverse = CAST(T, 1) / accuracy;
		const auto significantFigures = Floor(valuePlusDpsByAcc);
		const T roundedValue = CAST(T, significantFigures * accuracyInverse);
		return isNegative ? -roundedValue : roundedValue;
	}

	template<typename T, class = std::enable_if_t<
		!std::is_rvalue_reference_v<T>
		&& std::is_nothrow_move_assignable_v<T>
		&& std::is_nothrow_move_constructible_v<T>
		>>
		constexpr void Swap(T& lhs, T& rhs) noexcept
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
	USE_RESULT constexpr T Lerp(T a, T b, T t) noexcept
	{
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T LerpClampled(T a, T b, T t) noexcept
	{
		t = Clamp<T>(t, 0, 1);
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T LerpPartial(T a, T b, T t, T tmin, T tmax) noexcept
	{
		t = Clamp<T>(t, tmin, tmax);

		t -= tmin;
		t /= (tmax - tmin);

		return LerpClampled<T>(a, b, t);
	}

	template <typename T>
	USE_RESULT T GetAccelerationOverTime(T initialVelocity, T distance) noexcept
	{
		const T acceleration = (0 - (initialVelocity * initialVelocity)) / (2 * distance);

		return acceleration;
	}

	template <typename T>
	USE_RESULT constexpr T GetTimeTakenForAcceleration(T initialVelocity, T distance, T finalVelocity) noexcept
	{
		const T timeResult = (finalVelocity - initialVelocity) / GetAccelerationOverTime(initialVelocity, distance);

		return timeResult;
	}

	template <typename T>
	USE_RESULT constexpr T LerpWithAcceleration(T initialVelocity, T currentTime, T distance) noexcept
	{
		const T acceleration = GetAccelerationOverTime<T>(initialVelocity, distance);
		const T result = (initialVelocity * currentTime) + ((acceleration / 2) * (currentTime * currentTime));
		const T currentvel = initialVelocity + (acceleration * currentTime);
		return result;
	}



	template <typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T FloatingPointRemainder(T num, T base) noexcept
	{
		const auto isNegative = num < 0;

#if MSVC_PLATFORM_TOOLSET > 142
		return std::fmod(num, base);
#else
		const auto b = CAST(long double, base);
		const auto n = CAST(long double, num);

		const auto one_over_base = constants::OneOver<long double>(b);
		const auto num_over_base = n * one_over_base;
		const auto int_n_over_b = CAST(int, num_over_base);

		if (num_over_base == int_n_over_b)
			return 0;

		const auto closestMultiplier = int_n_over_b * b;
		const auto rem = n - closestMultiplier;

		return CAST(T, rem);
#endif
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Modulus(T num, T base) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			const auto mod = (num < 0)
				? FloatingPointRemainder(num, base) + base
				: FloatingPointRemainder(num, base);
			return mod;
		}
		else
		{
			T const rem = num % base;
			if _CONSTEXPR_IF(-1 % 2 == 1)
			{
				return rem;
			}
			else
			{
				const auto mod = rem < 0 ? rem + base : rem;
				return mod;
			}
		}
	}

	// Bakhshali Method
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T SqrtImpl(T square) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, sqrt(square));
#else
		auto maxIterations = 0;

		if _CONSTEXPR_IF(std::is_same_v<T, float>)
			maxIterations = 7;
		else
			maxIterations = 16;

		constexpr T lookUpMap[] = {
			CAST(T, 0),    CAST(T, 1),    CAST(T, 4),    CAST(T, 9),      // 0, 1, 2, 3 
			CAST(T, 16),   CAST(T, 25),   CAST(T, 36),   CAST(T, 49),     // 4, 5, 6, 7
			CAST(T, 64),   CAST(T, 81),   CAST(T, 100),  CAST(T, 121),    // 8, 9, 10, 11
			CAST(T, 144),  CAST(T, 169),  CAST(T, 196),  CAST(T, 225),    // 12, 13, 14, 15
			CAST(T, 256),  CAST(T, 289),  CAST(T, 324),  CAST(T, 361),    // 16, 17, 18, 19
			CAST(T, 400),  CAST(T, 441),  CAST(T, 484),  CAST(T, 529),    // 20, 21, 22, 23
			CAST(T, 576),  CAST(T, 625),  CAST(T, 676),  CAST(T, 729),    // 24, 25, 26, 27
			CAST(T, 784),  CAST(T, 841),  CAST(T, 900),  CAST(T, 961),    // 28, 29, 30, 31
			CAST(T, 1024), CAST(T, 1089), CAST(T, 1156), CAST(T, 1225) // 32, 33, 34, 35
		};

		if (square <= 0)
			return 0;

		if (square == constants::ZeroPointFive<T>())
			return CAST(T, constants::SQRT_1_OVER_2);

		if (square == constants::One<T>())
			return square;

		if (square == 2)
			return CAST(T, constants::ROOT2);

		const auto chooseStartValueFunc = [&]() -> T // Utilizes binary search if given square is between 0 and lookUpMap's size squared
		{
			const auto size = sizeof(lookUpMap) / sizeof(T);
			T estimate = CAST(T, BinarySearchClosestImpl(lookUpMap, square, 0, size - 1, size));
			if (!estimate || estimate == constants::MinusOne<T>())
			{
				estimate = square;
				do {
					estimate *= constants::ZeroPointFive<T>();
				} while (estimate * estimate > square);
			}
			return estimate;
		};

		T start = chooseStartValueFunc();

		if (start * start == square)
			return start;

		T result = start;
		T prevValue[2] = { constants::MinusOne<T>(), constants::MinusOne<T>() };

		const auto checkResultIsUnique = [&result, &prevValue]() {
			for (auto& prev : prevValue)
				if (prev == result)
					return false;
			return true;
		};

		while (checkResultIsUnique() && maxIterations > 0)
		{
			prevValue[Modulus(maxIterations, 2)] = result;
			--maxIterations;

			// Bakhshali Method
			const auto a = (square - (result * result)) / (2 * result);
			const auto b = result + a;
			result = b - ((a * a) / (2 * b));

			// Heron's Method
			//result = CAST(T, zeroPointFive * (result + (square / result)));
		}

		return result;
#endif
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T RootImpl(T num, size_t root)
	{
		const auto oneOverRoot = constants::OneOver<T>(root);
		constexpr auto minusZeroPointOne = -constants::ZeroPointOne<T>();

		if (num < 0)
		{
			if ((root & 1) == 0) // Even root
				throw std::runtime_error("No real root");

			if (num == constants::MinusOne<T>())
				return constants::MinusOne<T>();
		}

		if (num == 0)
			return 0;

		if (num == constants::One<T>() || root == constants::One<T>())
			return num;

		const auto chooseStartNumber = [&](auto number) -> T
		{
			auto maxIterations = 0;
			if _CONSTEXPR_IF(std::is_same_v<T, float>)
				maxIterations = 7;
			else
				maxIterations = 16;

			T estimate = 0;
			const bool isNegative = num < 0;

			if (isNegative)
				number = -num;

			if (num < 1 && num > -1)
			{
				T startVal = 0, endVal = 1;
				auto increment = number > 0 ? constants::ZeroPointOne<T>() : minusZeroPointOne;
				for (auto i = 0; i < maxIterations; ++i)
				{
					T compareVal = 0;
					do {
						endVal -= increment;
						estimate = (startVal + endVal) * constants::ZeroPointFive<T>();
						compareVal = PowerOfImpl<T>(estimate, root);
					} while (compareVal > number);

					startVal = estimate;
					endVal += increment;
					increment *= constants::ZeroPointOne<T>();

					if (compareVal == number)
						break;
				}
			}
			else
			{
				estimate = number;
				do {
					estimate *= oneOverRoot;
				} while (PowerOfImpl<T>(estimate, root) > number);
			}

			return isNegative ? -estimate : estimate;
		};

		const T start = chooseStartNumber(num);

		if (PowerOfImpl(start, root) == num)
			return start;

		auto increment = constants::One<T>();
		T result = start;
		T prev[2] = { constants::MinusOne<T>(), constants::MinusOne<T>() };

		const auto checkResultIsUnique = [&result, &prev]() {
			for (auto& p : prev)
				if (p == result)
					return false;
			return true;
		};

		auto iterations = 0;
		const int size = sizeof(prev) / sizeof(T);
		while (checkResultIsUnique())
		{
			prev[Modulus(iterations++, size)] = result;

			const auto x_power_of_root_minus_one = PowerOfImpl(result, root - 1);
			const auto num_over_x_pow_r_m_o = num / x_power_of_root_minus_one;
			const auto next_result = oneOverRoot * (num_over_x_pow_r_m_o - result);
			result += next_result;
		}

		return result;
	}

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4244)

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Root(T num, uint8_t root) noexcept
	{
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
			return CAST(T, RootImpl<float>(CAST(float, num), root));
		else
			return RootImpl<T>(num, root);
	}

	// Bakhshali Method
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Sqrt(T square) noexcept
	{
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
			return CAST(T, SqrtImpl<float>(square));
		else
			return SqrtImpl<T>(square);
	}


	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T InvSqrt(T square) noexcept
	{
		return constants::One<T>() / RootImpl<T>(square, 2);
	}

#	pragma warning(pop)
#endif

	template<typename T>
	USE_RESULT constexpr T PowerOf(T base, size_t numerator, size_t denominator) noexcept
	{
		if (denominator == 0)
			return 0;

		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			const auto pow = PowerOfImpl<float>(CAST(float, base), numerator);
			const auto powRoot = RootImpl<float>(pow, denominator);
			const auto result = CAST(T, powRoot);
			return result;
		}
		else
		{
			const auto result = RootImpl<T>(PowerOfImpl<T>(base, numerator), denominator);
			return result;
		}
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, T decimalPower) noexcept
	{
		const auto fraction = DecimalToFraction(decimalPower);
		const auto sign = fraction.GetSign();
		const auto numerator = fraction.GetNumerator();
		const auto denominator = fraction.GetDenominator();

		if (denominator == 0)
			return 0;

		const auto pow = PowerOfImpl<T>(base, numerator * sign);
		const auto powRoot = RootImpl<T>(pow, denominator);
		const auto result = powRoot;
		return result;
	}

	template<typename T, class = std::enable_if_t<!std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, long long power) noexcept
	{
		const auto pow = PowerOfImpl<T>(base, power);
		return pow;
	}
}

