#pragma once

#include "../../HelperMacros.hpp"
#include "kStringTypeTraits.hpp"
#include <string>

namespace klib::kString
{
	char16_t* UTF8ConvertToUTF16(const char* utf8);
	std::u16string UTF8ConvertToUTF16(const std::string_view& utf8Str);

	char32_t* UTF8ConvertToUTF32(const char* utf8);
	std::u32string UTF8ConvertToUTF32(const std::string_view& utf8Str);

	char* UTF16ConvertToUTF8(const char16_t* utf16);
	std::string UTF16ConvertToUTF8(const std::u16string_view& utf16Str);

	char32_t* UTF16ConvertToUTF32(const char16_t* utf16);
	std::u32string UTF16ConvertToUTF32(const std::u16string_view& utf16Str);

	char* UTF32ConvertToUTF8(const char32_t* utf32);
	std::string UTF32ConvertToUTF8(const std::u32string_view& utf32Str);

	char16_t* UTF32ConvertToUTF16(const char32_t* utf32);
	std::u16string UTF32ConvertToUTF16(const std::u32string_view& utf32Str);


	template<typename SourceChar, typename DestChar, typename U1 = SourceChar, typename U2 = DestChar>
	USE_RESULT std::enable_if_t<
		type_trait::Is_CharType_V<U1>
		&& type_trait::Is_CharType_V<U2>> Convert(const SourceChar* source, DestChar*& dest) noexcept
	{
		if (dest)
			delete[] dest;

		if _CONSTEXPR_IF(std::is_same_v<SourceChar, char>
			&& std::is_same_v<DestChar, char16_t>)
			dest = UTF8ConvertToUTF16(source);
		else if _CONSTEXPR_IF(std::is_same_v<SourceChar, char>
			&& std::is_same_v<DestChar, char32_t>)
			dest = UTF8ConvertToUTF32(source);
		else if _CONSTEXPR_IF(std::is_same_v<SourceChar, char16_t>
			&& std::is_same_v<DestChar, char32_t>)
			dest = UTF16ConvertToUTF32(source);
		else if _CONSTEXPR_IF(std::is_same_v<SourceChar, char16_t>
			&& std::is_same_v<DestChar, char>)
			dest = UTF16ConvertToUTF8(source);
		else if _CONSTEXPR_IF(std::is_same_v<SourceChar, char32_t>
			&& std::is_same_v<DestChar, char16_t>)
			dest = UTF32ConvertToUTF16(source);
		else if _CONSTEXPR_IF(std::is_same_v<SourceChar, char32_t>
			&& std::is_same_v<DestChar, char>)
			dest = UTF32ConvertToUTF8(source);
	}
}