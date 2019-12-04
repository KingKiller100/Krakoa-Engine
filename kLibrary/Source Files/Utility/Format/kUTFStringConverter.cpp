#include <pch.hpp>
#include <Utility/Format/kUTFStringConverter.hpp>

// TO BE IMPLEMENTED - NEED TO LEARN MORE ABOUT UTF/UNICODE AND CHARACTER ENCODING

namespace util::kUTF
{
	std::u16string UTF8ConvertToUTF16(const std::string_view utf8Str)
	{
		return UTF8ConvertToUTF16(utf8Str.data());
	}

	const char16_t* UTF8ConvertToUTF16(const char* utf8)
	{
		return u"";
	}

	const char32_t* UTF8ConvertToUTF32(const char* utf8)
	{
		return U"";
	}

	std::u32string UTF8ConvertToUTF32(const std::string_view utf8Str)
	{
		return UTF8ConvertToUTF32(utf8Str.data());
	}

	const char* UTF16ConvertToUTF8(const char16_t* utf16)
	{
		return "";
	}

	std::string UTF16ConvertToUTF8(const std::u16string_view utf16Str)
	{
		return UTF16ConvertToUTF8(utf16Str.data());
	}

	const char32_t* UTF16ConvertToUTF32(const char16_t* utf16)
	{
		return U"";
	}

	std::u32string UTF16ConvertToUTF32(const std::u16string_view utf16Str)
	{
		return UTF16ConvertToUTF32(utf16Str.data());
	}

	const char* UTF32ConvertToUTF8(const char32_t* utf32)
	{
		return "";
	}

	std::string UTF32ConvertToUTF8(const std::u32string_view utf32Str)
	{
		return UTF32ConvertToUTF8(utf32Str.data());
	}

	const char16_t* UTF32ConvertToUTF16(const char32_t* utf32)
	{
		return u"";
	}

	std::u16string UTF32ConvertToUTF16(const std::u32string_view utf32Str)
	{
		return UTF32ConvertToUTF16(utf32Str.data());
	}
}
