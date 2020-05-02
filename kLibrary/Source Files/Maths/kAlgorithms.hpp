#pragma once

#include "../HelperMacros.hpp"

#include "Constants.hpp"

#include "../Utility/Debug Helper/Exceptions/NotImplementedException.hpp"

#if MSVC_PLATFORM_TOOLSET > 142
#	include <cmath>
#endif

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
		constexpr auto maxLL = std::numeric_limits<long long>::max();
		constexpr auto maxLD = std::numeric_limits<T>::max();

		if (value > maxLL)
			return value;

		return CAST(T, CAST(long long, value));
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
	USE_RESULT constexpr decltype(auto) PowerOf10(int power) noexcept(std::is_arithmetic_v<T>)
	{
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return 0;
		}
		else
		{
			return PowerOf<T>(10, power);
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

	USE_RESULT constexpr int WhatPowerOf10(double number) noexcept
	{
		if (number < 1 && number > 0.1)
			return -1;
		if (number >= 1 && number < 10)
			return 0;
		if (number >= 10 && number < 100)
			return 1;

		if (number < 0)
			number = -number;

		int currentPower = 1;
		if (number > 10)
		{
			long long value = 1;
			while (value < number)
			{
				currentPower++;
				value = PowerOf(10, currentPower);
			}
			--currentPower;
		}
		else if (number < 1)
		{
			long double value = 0.1;
			while (value > number)
			{
				currentPower++;
				value = PowerOf(0.1, currentPower);
			}
			currentPower = -currentPower;
		}

		return currentPower;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Round(const T value, const uint8_t decimalPoints) noexcept
	{
		const auto dpShifts = PowerOf<long double>(0.1, decimalPoints + 1) * 5;
		const auto accuracy = PowerOf<size_t>(10, decimalPoints);

		const auto valuePlusDpsByAcc = (value + dpShifts) * accuracy;
		const auto accuracyInverse = CAST(T, 1) / accuracy;
		const auto significantFigures = Floor(valuePlusDpsByAcc);
		const T roundedValue = CAST(T, significantFigures * accuracyInverse);
		return roundedValue;
	}

	template<typename T, class = std::enable_if_t<!std::is_rvalue_reference_v<T>>>
	constexpr void Swap(T& lhs, T& rhs) noexcept(std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_constructible_v<T>)
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
	USE_RESULT constexpr T FloatingPointModulus(T num, T base) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return std::fmod(num, base);
#else
		const auto isNegative = num < 0;

		const auto one_over_base = CAST(T, 1) / base;
		const auto num_over_base = num * one_over_base;
		const auto int_n_over_b = CAST(int, num_over_base);

		if (num_over_base == int_n_over_b)
			return 0;

		const auto closestMultiplier = int_n_over_b * base;
		const auto mod = num - closestMultiplier;

		return mod;
#endif
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Modulus(T num, T base) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			const auto mod = (num < 0)
				? FloatingPointModulus(num, base) + base
				: FloatingPointModulus(num, base);
			return mod;
		}
		else
		{
			T const rem = num % base;
			if _CONSTEXPR_IF(-1 % 2 == 1)
			{
				return rem;
			}

			const auto mod = rem < 0 ? rem + base : rem;
			return mod;
		}
	}

	// Bakhshali Method
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T SqrtImpl(T square) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, sqrt(square));
#else
		constexpr auto one = CAST(T, 1);
		constexpr auto minusOne = CAST(T, -1);
		constexpr auto zeroPointOne = CAST(T, 0.1);
		constexpr auto zeroPointFive = CAST(T, 0.5);
		auto maxIterations = 0;

		if _CONSTEXPR_IF(std::is_same_v<T, float>)
			maxIterations = 7;
		else
			maxIterations = 16;

		constexpr T lookUpMap[] = {
			CAST(T, 0),    // 0 
			CAST(T, 1),    // 1 
			CAST(T, 4),    // 2 
			CAST(T, 9),	   // 3
			CAST(T, 16),   // 4
			CAST(T, 25),   // 5
			CAST(T, 36),   // 6
			CAST(T, 49),   // 7
			CAST(T, 64),   // 8
			CAST(T, 81),   // 9
			CAST(T, 100),  // 10
			CAST(T, 121),  // 11
			CAST(T, 144),  // 12
			CAST(T, 169),  // 13
			CAST(T, 196),  // 14
			CAST(T, 225),  // 15
			CAST(T, 256),  // 16
			CAST(T, 289),  // 17
			CAST(T, 324),  // 18
			CAST(T, 361),  // 19
			CAST(T, 400),  // 20
			CAST(T, 441),  // 21
			CAST(T, 484),  // 22
			CAST(T, 529),  // 23
			CAST(T, 576),  // 24
			CAST(T, 625),  // 25
			CAST(T, 676),  // 26
			CAST(T, 729),  // 27
			CAST(T, 784),  // 28
			CAST(T, 841),  // 29
			CAST(T, 900),  // 30
			CAST(T, 961),  // 31
			CAST(T, 1024), // 32
		};

		if (square <= 0)
			return 0;

		if (square == zeroPointFive)
			return CAST(T, constants::SQRT_1_OVER_2);

		if (square == one)
			return square;

		if (square == 2)
			return CAST(T, constants::ROOT2);

		const auto chooseStartValueFunc = [&]() -> T // Utilizes binary search if given square is between 0 and lookUpMap's size squared
		{
			const auto size = sizeof(lookUpMap) / sizeof(T);
			T estimate = CAST(T, BinarySearchClosestImpl(lookUpMap, square, 0, size - 1, size));
			if (estimate == minusOne)
			{
				estimate = square;
				do {
					estimate *= zeroPointFive;
				} while (estimate * estimate > square);
			}
			return estimate;
		};

		T start = chooseStartValueFunc();

		if (start * start == square)
			return start;

		T result = start;
		T prevValue[2] = { minusOne, minusOne };

		const auto checkPrevResultsMatch = [&result, &prevValue]() {
			for (auto& prev : prevValue)
				if (prev == result)
					return false;
			return true;
		};

		while (checkPrevResultsMatch() && maxIterations > 0)
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
	USE_RESULT constexpr T RootImpl(T num, uint8_t root)
	{
		constexpr auto one = CAST(T, 1);
		const auto oneOverRoot = one / root;
		constexpr auto minusOne = CAST(T, -1);
		constexpr auto zeroPointOne = CAST(T, 0.1);
		constexpr auto minusZeroPointOne = CAST(T, 0.1);
		constexpr auto zeroPointFive = CAST(T, 0.5);

		if (num < 0)
		{
			if ((root & 1) == 0) // Even root
				throw std::runtime_error("No real root");

			if (num == minusOne)
				return minusOne;
		}

		if (num == 0)
			return 0;

		if (num == one)
			return one;

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
				auto increment = number > 0 ? zeroPointOne : minusZeroPointOne;
				for (auto i = 0; i < maxIterations; ++i)
				{
					T compareVal = 0;
					do {
						endVal -= increment;
						estimate = (startVal + endVal) * zeroPointFive;
						compareVal = PowerOf<T>(estimate, root);
					} while (compareVal > number);

					startVal = estimate;
					endVal += increment;
					increment *= zeroPointOne;

					if (compareVal == number)
						break;
				}
			}
			else
			{
				estimate = number;
				do {
					estimate *= oneOverRoot;
				} while (PowerOf<T>(estimate, root) > number);
			}

			return isNegative ? -estimate : estimate;
		};

		const T start = chooseStartNumber(num);

		if (PowerOf(start, root) == num)
			return start;

		auto increment = one;
		T result = start;
		T prev[2] = { minusOne, minusOne };

		const auto checkUniqueResult = [&result, &prev]() {
			for (auto& p : prev)
				if (p == result)
					return false;
			return true;
		};

		auto iterations = 0;
		const int size = sizeof(prev) / sizeof(T);
		while (checkUniqueResult())
		{
			prev[Modulus(iterations++, size)] = result;
			const auto x_power_of_root_minus_one = PowerOf(result, root - 1);
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
		constexpr auto one = CAST(T, 1);
		return one / RootImpl<T>(square, 2);
	}

#	pragma warning(pop)
#endif

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, int8_t numerator, uint8_t denominator) noexcept
	{
		if (denominator == 0)
			return 0;

		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			const auto pow = PowerOf<float>(CAST(float, base), numerator);
			const auto powRoot = RootImpl<float>(pow, denominator);
			const auto result = CAST(T, powRoot);
			return result;
		}
		else
		{
			const auto result = RootImpl<T>(PowerOf<T>(base, numerator), denominator);
			return result;
		}

	}
}