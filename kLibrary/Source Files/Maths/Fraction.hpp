#pragma once

#include "../HelperMacros.hpp"

#include "Length_Type.hpp"

#include <cstdint>

namespace kmaths
{
	struct Fraction
	{
	public:
		using Sign_Value_Type = bool;
		using Numerator_Value_Type = size_t;
		using Denominator_Value_Type = size_t;

		constexpr Fraction(const Numerator_Value_Type numerator = 0, const Denominator_Value_Type denominator = 1, const Sign_Value_Type isNegative = false) noexcept
			: numerator(numerator), denominator(denominator), isNegative(isNegative)
		{
			Simplify();
		}

		~Fraction() noexcept
			= default;

		template<typename T>
		USE_RESULT constexpr T GetReal() const noexcept
		{
			if _CONSTEXPR_IF(std::is_signed_v<T>)
			{
				const T num = CAST(T, numerator);
				const T signedNum = isNegative ? -num : num;
				return  signedNum / CAST(T, denominator);
			}
			else
			{
				const T num = CAST(T, isNegative ? 0 : numerator);
				return  num / CAST(T, denominator);
			}
		}

		constexpr void Simplify() noexcept
		{
			const auto lcf = LargestCommonFactor();
			const auto oneOverLCF = 1.f / lcf;
			numerator = CAST(Numerator_Value_Type, oneOverLCF * numerator);
			denominator = CAST(Numerator_Value_Type, oneOverLCF * denominator);
		}

		// Operators
		USE_RESULT constexpr Fraction operator+(const Fraction& other) const noexcept
		{
			const auto den = denominator * other.denominator;

			const auto num = numerator * other.denominator;
			const auto oNum = other.numerator * denominator;

			const auto sameSign = (!isNegative && !other.isNegative)
				|| (isNegative && other.isNegative);

			if (sameSign)
			{
				const auto n_Add_oN = num + oNum;
				Fraction f(n_Add_oN, den, isNegative);
				f.Simplify();
				return f;
			}
			else
			{
				const auto big = num > oNum ? num : oNum;
				const auto small = num < oNum ? num : oNum;
				const auto newSign = big == num ? isNegative : other.isNegative;
				const auto n = big - small;
				Fraction f(n, den, newSign);
				f.Simplify();
				return f;
			}
		}

	private:
		USE_RESULT constexpr float LargestCommonFactor() const noexcept
		{
			float largestFactor = 1;
			const auto largest = numerator > denominator ? numerator : denominator;
			const auto loops = (largest / 2) + 1;

			for (Denominator_Value_Type i = 2; i < loops; ++i)
			{
				const double multiplier = 1.0 / i;
				const auto numFactor = double(numerator) * multiplier;
				const auto denFactor = double(denominator) * multiplier;

				if (Big_Int_Type(numFactor) == numFactor
					&& Big_Int_Type(denFactor) == denFactor)
				{
					largestFactor = float(i);
				}
			}

			return largestFactor;
		}

	public:
		Numerator_Value_Type numerator;
		Denominator_Value_Type denominator;

		Sign_Value_Type isNegative;
	};
}


