#pragma once

#include "../HelperMacros.hpp"

#include "Fraction.hpp"
#include "Constants.hpp"
#include "Length_Type.hpp"

#if MSVC_PLATFORM_TOOLSET > 142
#	include <cmath>
#endif

#include <cstdint>
#include <type_traits>

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

	template<typename T, size_t Size>
	USE_RESULT constexpr size_t SizeOfCArray(const T(&array)[Size]) noexcept
	{
		return Size;
	}

	template<typename List, typename T>
	USE_RESULT constexpr Big_Int_Type BinarySearchImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
	{
		if (lbIdx > ubIdx
			|| value < list[0]
			|| value > list[size - 1])
			return -1;

		const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);
		const auto mid = list[midIdx];

		if (mid > value)
			return BinarySearchImpl(list, value, lbIdx, midIdx - 1, size);
		if (mid < value)
			return BinarySearchImpl(list, value, midIdx + 1, ubIdx, size);

		return midIdx;
	}

	template<typename T>
	USE_RESULT constexpr Big_Int_Type BinarySearch(T* list, T&& value, size_t size) noexcept
	{
		return BinarySearchImpl(list, value, 0, size - 1, size);
	}

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr Big_Int_Type BinarySearch(const T(&list)[N], T&& value) noexcept
	{
		return BinarySearchImpl(list, value, 0, N - 1, N);
	}

	template<typename List, typename T>
	USE_RESULT constexpr Big_Int_Type BinarySearchClosestImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
	{
		if (lbIdx > ubIdx
			|| value < list[0]
			|| value > list[size - 1])
			return -1;

		const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);

		const auto lowerBound = list[midIdx];
		const auto upperBound = list[midIdx + 1];

		if (lowerBound == value)
			return midIdx;
		else if (value < lowerBound)
			return BinarySearchClosestImpl(list, value, lbIdx, midIdx - 1, size);
		else if (value > upperBound)
			return BinarySearchClosestImpl(list, value, midIdx + 1, ubIdx, size);

		const auto lbDiff = value - lowerBound;
		const auto ubDiff = upperBound - value;

		return lbDiff < ubDiff ? midIdx : midIdx + 1;
	}

	template< typename T>
	USE_RESULT constexpr Big_Int_Type BinarySearchClosest(const T* list, T&& value, size_t size)
	{
		return BinarySearchClosestImpl(list, value, 0, size - 1, size);
	};

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr Big_Int_Type BinarySearchClosest(const T(&list)[N], T&& value)
	{
		return BinarySearchClosestImpl(list, value, 0, N - 1, N);
	};

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsInteger(T value) noexcept
	{
		return Convert<int>(value) == value;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr unsigned int CountIntegerDigits(T x) noexcept
	{
		unsigned int count = 1;
		bool stop = CAST(T, 10) > x&& x > CAST(T, -10);

		while (!stop)
		{
			x /= CAST(T, 10);
			count++;
			stop = CAST(T, 10) > x&& x > CAST(T, -10);
		}

		return count;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsDecimal(T value) noexcept
	{
		constexpr auto one = constants::One<T>();
		constexpr auto minusOne = constants::MinusOne<T>();

		return ((value > minusOne)
			&& (value < one));
	}

	template<typename T1, typename T2>
	USE_RESULT constexpr T1 Max(const T1& lhs, const T2& rhs) noexcept
	{
		return lhs > rhs ? lhs : CAST(T1, rhs);
	}

	template<typename T1, typename T2>
	USE_RESULT constexpr T1 Min(const T1& lhs, const T2& rhs) noexcept
	{
		return lhs < rhs ? lhs : CAST(T1, rhs);
	}

	template<typename T>
	USE_RESULT constexpr T Fibonacci(const T n) noexcept
	{
		if (n <= 1)
			return n;

		return (Fibonacci(n - 1) + Fibonacci(n - 2));
	}


	// Sign///////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT constexpr uint8_t Sign_Impl(const T x, std::false_type) noexcept
	{
		return (T(0) < x);
	}
	template<typename T>
	USE_RESULT constexpr int8_t Sign_Impl(const T x, std::true_type) noexcept
	{
		return (T(0) < x) - (x < T(0));
	}
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr decltype(auto) Sign(const T x) noexcept
	{
		return Sign_Impl(x, std::is_signed<T>());
	}
	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	USE_RESULT constexpr bool IsNegative(T x) noexcept
	{
		return x < 0;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Floor(const T value) noexcept
	{
		constexpr auto maxVal = std::numeric_limits<Big_Int_Type>::max();
		constexpr auto minVal = std::numeric_limits<Big_Int_Type>::min();

		if (value > maxVal || value < minVal)
			return value;

		const auto integer = CAST(T, CAST(Big_Int_Type, value));

		return integer > value ? integer - CAST(T, 1) : integer;
	}

	template<typename T>
	USE_RESULT constexpr T Abs(const T x) noexcept
	{
		if _CONSTEXPR_IF(std::is_unsigned_v<T>)
			return x;
		else
		{
			if (x >= 0)
				return x;
			else
			{
				if _CONSTEXPR_IF(std::is_integral_v<T>)
					return (~x + CAST(T, 1));
				else
					return -x;
			}
		}
	}

	// https://stackoverflow.com/questions/34703147/sine-function-without-any-library/34703167
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T SineImpl(T x, const size_t n) noexcept
	{
		constexpr auto one = constants::One<constants::AccuracyType>();
		constexpr auto two = one + one;
		const auto square = CAST(constants::AccuracyType, -x * x);

		auto t = CAST(constants::AccuracyType, x);
		auto sine = t;
		for (size_t a = 1; a < n; ++a)
		{
			const constants::AccuracyType xn = square / ((two * a + one) * (two * a));
			t *= xn;
			sine += t;
		}
		return CAST(T, sine);
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Sine(T x, const size_t n = 50) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
			return SineImpl<T>(x, n);
		else
			return CAST(T, SineImpl<float>(CAST(float, x), n));
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Cosine(T x, const size_t n = 50) noexcept
	{
		constexpr auto pi_over_2 = CAST(T, constants::PI_OVER_2);
		x += pi_over_2;

		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
			return SineImpl<T>(x, n);
		else
			return CAST(T, SineImpl<float>(CAST(float, x), n));
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Tan(T x, const size_t n = 20) noexcept
	{
		const auto sine = Sine(x, n);
		const auto cosine = Cosine(x, n);

		return (sine / cosine);
	}

	// Continued Fractions Without Tears, Ian Richards 1981
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr Fraction RealToFraction(T x, const uint8_t dpAccuracy = 10) noexcept
	{
		constexpr auto maxIterations = Big_Int_Type(1e6);
		Big_Int_Type iter = 0;

		const auto isNegative = x < 0;
		if (isNegative) x = -x;

		if (Big_Int_Type(x) == x)
			return { Fraction::Numerator_Value_Type(x), 1, isNegative, false };

		const T error = PowerOfImpl(constants::ZeroPointOne<T>(), Min(dpAccuracy, Max_DP_Precision<T>));

		const T x0 = x;
		size_t a(0);
		size_t b(1);
		size_t c(1);
		size_t d(0);
		size_t integer(0);

		do {
			integer = CAST(size_t, x);
			const Fraction::Numerator_Value_Type num = a + integer * c;
			const Fraction::Denominator_Value_Type den = b + integer * d;
			a = c;
			b = d;
			c = num;
			d = den;
			x = constants::OneOver<T>(x - CAST(T, integer));
			const auto diff = Abs<T>(x0 - (CAST(T, num) / den));
			if (error > diff) { return { num, den, isNegative, false }; }
		} while (iter++ < maxIterations);

		return { 0, 1, isNegative, false };
	}


	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T HandleFloatingPointError(T x) noexcept
	{
		if (x == 0)
			return 0;

		const auto x0 = x;

		constexpr auto epsilon = std::numeric_limits<T>::epsilon() * 10;

		const auto isNegative = x < 0;

		const auto integer = Floor<T>(x);
		const auto isDecimals = IsDecimal<T>(x);

		if (!isDecimals)
		{
			x = isNegative
				? x + integer
				: x - integer;
		}

		if (x != 0 && (epsilon >= x && x >= -epsilon))
			return x0 - x;
		else if (integer != x0)
		{
			if (std::is_same_v<T, float>)
				return Round(x0, 5);
			else
				return Round(x0, 9);
		}
		else
			return x0;
	}

	template<typename T>
	USE_RESULT constexpr T PowerOfImpl(T base, Big_Int_Type power) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return CAST(T, pow(base, power));
#else
		T temp = T();

		if (power == 0)
			return constants::One<T>();

		temp = PowerOfImpl(base, power / 2);

		if (power % 2 == 0)
			return temp * temp;
		else if (power > 0)
			return base * temp * temp;
		else
			return (temp * temp) / base;
#endif
	}

	template<typename T>
	USE_RESULT constexpr T PowerOf10(T power) noexcept(std::is_arithmetic_v<T>)
	{
		constexpr auto ten = CAST(T, 10);
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return 0;
		}
		return PowerOfImpl<T>(ten, power);
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Round(T value, const uint8_t decimalPoints) noexcept
	{
		const auto isNegative = value < 0;
		if (isNegative)
			value = -value;

		const auto dpShifts = PowerOfImpl<long double>(constants::ZeroPointOne<T>(), (decimalPoints + 1)) * 5;
		const auto accuracy = PowerOfImpl<size_t>(10, decimalPoints);

		const auto valuePlusDpsByAcc = (value + dpShifts) * accuracy;
		const auto accuracyInverse = constants::OneOver<T>(accuracy);
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
		constexpr auto convertR2D = constants::RadiansToDegrees<T>();
		return CAST(T, radians * convertR2D);
	}

	template<typename T>
	USE_RESULT constexpr T ToRadians(const T degrees) noexcept
	{
		constexpr auto convertD2R = constants::DegreesToRadians<constants::AccuracyType>();
		return CAST(T, degrees * convertD2R);
	}

	template<typename T>
	USE_RESULT constexpr T Clamp(const T value, const T min, const T max)
	{
		if (max <= min) throw std::exception();

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
		constexpr auto lookUpMapSize = SizeOfCArray(lookUpMap);

		auto maxIterations = 0;

		if _CONSTEXPR_IF(std::is_same_v<T, float>)
			maxIterations = 7;
		else
			maxIterations = 16;

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
			T estimate = CAST(T, BinarySearchClosestImpl(lookUpMap, square, 0, lookUpMapSize - 1, lookUpMapSize));
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

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
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

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, T power) noexcept
	{
		const auto fraction = RealToFraction<T>(power);

		const auto pow = fraction.isNegative
			? constants::OneOver<T>(PowerOfImpl<T>(base, fraction.numerator))
			: PowerOfImpl<T>(base, fraction.numerator);
		const auto powRoot = RootImpl<T>(pow, fraction.denominator);
		return powRoot;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, size_t numerator, size_t denominator, bool isNegative = false) noexcept
	{
		if (denominator == 0)
			return 0;

		const int8_t sign = isNegative ? -1 : 1;

		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			const auto pow = PowerOfImpl<float>(base, numerator * sign);
			const auto powRoot = RootImpl<float>(pow, denominator);
			return CAST(T, powRoot);
		}
		else
		{
			const auto pow = PowerOfImpl<T>(base, numerator * sign);
			const auto powRoot = RootImpl<T>(pow, denominator);
			return powRoot;
		}
	}

	template<typename T, class = std::enable_if_t<!std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, Big_Int_Type power) noexcept
	{
		const auto pow = PowerOfImpl<T>(base, power);
		return pow;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Log(const T number) noexcept
	{
		constexpr auto one = constants::One<T>();
		constexpr auto zeroPointOne = constants::ZeroPointOne<T>();

		constexpr auto maxIter = CAST(unsigned short, 1e3);
		[[maybe_unused]] unsigned short currentIter = 0;

		if (number < 1 && number > zeroPointOne)
			return -1;
		if (number >= 1 && number < 10)
			return 0;
		if (number >= 10 && number < 100)
			return 1;

		if (number < 0)
			return std::numeric_limits<T>::max();

		Big_Int_Type currentPower = 0;
		Big_Int_Type minorIncrement = 1;
		const T base = number >= one ? CAST(T, 10) : zeroPointOne;

		bool found = false;

		if (number >= one)
		{
			while (PowerOfImpl(base, currentPower) < number)
			{
				currentPower += minorIncrement;
			};
			--currentPower;
		}
		else
		{
			while (PowerOfImpl(base, currentPower) > number)
			{
				currentPower += minorIncrement;
			};
			currentPower = -currentPower;
		}

		return CAST(T, currentPower);
	}

	// Approximation for the mathematical constant 'e^x' using the first n terms of the taylor series
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T ExponentialImpl(T x) noexcept
	{
		constexpr T e = CAST(T, constants::E);
		
		return PowerOf<T>(e, x);
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Exponential(T x) noexcept
	{
		if _CONSTEXPR_IF(std::is_integral_v<T>)
			return CAST(T, ExponentialImpl<float>(CAST(float, x)));
		else
			return ExponentialImpl<T>(x);
	}

	template<typename T>
	USE_RESULT constexpr T Square(T x) noexcept
	{
		return x * x;
	}

	template<typename T>
	USE_RESULT constexpr T Cube(T x) noexcept
	{
		return x * x * x;
	}

	template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
	USE_RESULT constexpr T Factorial_Integral(T n) noexcept
	{
		if (IsNegative(n)) return 0;

		if (n > 1)
			return (n * Factorial_Integral(n - 1));

		return 1;
	}

	// Lanczos' formula
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Gamma(T z) noexcept
	{
		constexpr double pi = constants::PI;
		constexpr double tau = constants::TAU;
		constexpr auto epsilon = std::numeric_limits<double>::epsilon();

		constexpr auto handleEpsilon = [epsilon](double value)
		{
			if (value <= epsilon)
				return 0.0;
			return value;
		};

		// accurate to about 15 decimal places
		//some magic constants 
		const auto g = 7; // g represents the precision desired, p is the values of p[i] to plug into Lanczos' formula
		double p[] = {
			0.99999999999980993, 676.5203681218851, -1259.1392167224028, 
			771.32342877765313, -176.61502916214059, 12.507343278686905,
			-0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7 };

		constexpr auto coefficientsSize = SizeOfCArray(p);

		double result = 0;

		if (z < 0.5)
			result = pi / Sine(z * pi) * Gamma(1.0 - z);
		else
		{
			z -= 1.0;
			auto x = p[0];
			for (auto i = 1; i < coefficientsSize; i++) {
				x += p[i] / (z + i );
			}
			const auto t = z + coefficientsSize - 0.5;
			result = Sqrt(tau) * PowerOf(t, (z + 0.5)) * Exponential(-t) * x;
		}

		return CAST(T, handleEpsilon(result));
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Factorial_Floating_Point(T n) noexcept
	{
		return Gamma(n + 1);
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Factorial(T n)
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
			return Factorial_Floating_Point<T>(n);
		else
			return Factorial_Integral<T>(n);
	}


}

