#pragma once

#include "../HelperMacros.hpp"

#include <xtr1common>
#include <cstdint>

namespace kmaths
{
	struct Fraction
	{
	public:
		using Sign_Value_Type = bool;
		using Numerator_Value_Type = size_t;
		using Denominator_Value_Type = size_t;

		constexpr Fraction(const Numerator_Value_Type numerator = 0, const Denominator_Value_Type denominator = 1, const bool isNegative = false) noexcept
			: numerator(numerator), denominator(denominator), isNegative(isNegative)
		{}

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

		Numerator_Value_Type numerator;
		Denominator_Value_Type denominator;

		Sign_Value_Type isNegative;
	};
}


