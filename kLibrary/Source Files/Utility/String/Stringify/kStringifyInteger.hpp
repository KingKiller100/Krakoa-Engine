#pragma once

#include <cstddef>

#include "kStringifyHelper.hpp"
#include "../kStringManipulation.hpp"

namespace klib::kString::stringify
{
	/// Digits of largest conceivable number for any integral type
	/// plus a null terminator + possible minus symbol
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr std::size_t max_digits = std::numeric_limits<T>::digits10 + 2;

	template<class CharType, typename Uint_t
		, typename = std::enable_if_t<std::is_unsigned_v<Uint_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		constexpr CharType* UintToStr(CharType* current, Uint_t uVal)
	{
		do {
			const auto mod = uVal % 10;
			*(--current) = static_cast<CharType>('0' + mod);
			uVal /= 10;
		} while (uVal != 0);
		return current;
	}

	template<class CharType, typename Signed_t
		, typename = std::enable_if_t<std::is_integral_v<Signed_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		kString::StringWriter<CharType> StringSignedIntegral(const Signed_t val, size_t minDigits)
	{
		if (minDigits == nPrecision)
			minDigits = 1;
		using unsigned_t = std::make_unsigned_t<Signed_t>;
		CharType buff[max_digits<Signed_t>]{};
		CharType* const end = std::end(buff);
		CharType* current = end;
		const auto uVal = static_cast<unsigned_t>(val);

		if (val < 0)
		{
			current = UintToStr(current, 0 - uVal);
			*(--current) = CharType('-');
		}
		else
		{
			current = UintToStr(current, uVal);
		}
		
		kString::StringWriter<CharType> str(current, end);
		PrependPadding(str, minDigits, CharType('0'));

		return str;
	}

	template<class CharType, typename Unsigned_t
		, typename = std::enable_if_t<std::is_integral_v<Unsigned_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		StringWriter<CharType> StringUnsignedIntegral(const Unsigned_t val, size_t minDigits)
	{
		if (minDigits == nPrecision)
			minDigits = 1;
		
		CharType buff[max_digits<Unsigned_t>]{};
		CharType* const end = std::end(buff);
		CharType* current = UintToStr(end, val);


		kString::StringWriter<CharType> str(current, end);
		PrependPadding(str, minDigits, CharType('0'));

		return str;
	}


	template<class CharType, typename Integral_t
		, typename = std::enable_if_t < std::is_integral_v<Integral_t>
	>>
		StringWriter<CharType> StringIntegral(const Integral_t val, size_t minDigits)
	{
		if constexpr (std::is_unsigned_v<Integral_t>)
			return StringUnsignedIntegral<CharType, Integral_t>(val, minDigits);
		else
			return StringSignedIntegral<CharType, Integral_t>(val, minDigits);
	}
}
