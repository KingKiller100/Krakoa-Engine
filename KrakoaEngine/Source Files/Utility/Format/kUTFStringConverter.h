#pragma once

#include <string>

namespace util::kUTF
{
	const char16_t* UTF8ConvertToUTF16(const char* utf8);
	std::u16string UTF8ConvertToUTF16(const std::string_view utf8Str);
	
	const char32_t* UTF8ConvertToUTF32(const char* utf8);
	std::u32string UTF8ConvertToUTF32(const std::string_view utf8Str);

	const char* UTF16ConvertToUTF8(const char16_t* utf16);
	std::string UTF16ConvertToUTF8(const std::u16string_view utf16Str);

	const char32_t* UTF16ConvertToUTF32(const char16_t* utf16);
	std::u32string UTF16ConvertToUTF32(const std::u16string_view utf16Str);

	const char* UTF32ConvertToUTF8(const char32_t* utf32);
	std::string UTF32ConvertToUTF8(const std::u32string_view utf32Str);

	const char16_t* UTF32ConvertToUTF16(const char32_t* utf32);
	std::u16string UTF32ConvertToUTF16(const std::u32string_view utf32Str);
	
}