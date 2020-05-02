#pragma once

#include "../HelperMacros.hpp"

#include <xtr1common>
#include <cstdint>

namespace kmaths
{
	struct Fraction
	{
	public:
		using Sign_Value_Type = int8_t;
		using Numerator_Value_Type = size_t;
		using Denominator_Value_Type = size_t;

		constexpr Fraction(const Numerator_Value_Type numerator = 0, const Denominator_Value_Type denominator = 1, const bool isNegative = true) noexcept
			: sign(isNegative ? -1 : 1), numerator(numerator), denominator(denominator)
		{}

		~Fraction()
			= default;

		template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr T GetDecimal() const noexcept
		{
			const auto signedVal = CAST(T, sign) * numerator;
			return signedVal / denominator;
		}

		USE_RESULT constexpr Sign_Value_Type GetSign() const noexcept
		{
			return sign;
		}

		USE_RESULT constexpr Numerator_Value_Type GetNumerator() const noexcept
		{
			return numerator;
		}

		USE_RESULT constexpr Denominator_Value_Type GetDenominator() const noexcept
		{
			return denominator;
		}

	private:
		Sign_Value_Type sign;
		Numerator_Value_Type numerator;
		Denominator_Value_Type denominator;
	};

}


