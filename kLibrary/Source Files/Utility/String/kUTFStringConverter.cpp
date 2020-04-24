#include "pch.hpp"
#include "kUTFStringConverter.hpp"

// TODO: TO BE IMPLEMENTED - NEED TO LEARN MORE ABOUT UTF/UNICODE AND CHARACTER ENCODING

namespace klib::kString
{
	std::u16string UTF8ConvertToUTF16(const std::string_view& utf8Str)
	{
		return UTF8ConvertToUTF16(utf8Str.data());
	}

	char16_t* UTF8ConvertToUTF16(const char* utf8)
	{
		const auto length = std::strlen(utf8);

		auto buffer = new char16_t[length + 1]{ u'\0' };
		for (auto i = 0; i < length; ++i)
		{
			buffer[i] = CAST(char16_t, utf8[i]);
		}

		return buffer;
	}

	char32_t* UTF8ConvertToUTF32(const char* utf8)
	{
		const auto length = std::strlen(utf8);

		auto buffer = new char32_t[length + 1]{ U'\0' };
		for (auto i = 0; i < length; ++i)
		{
			buffer[i] = CAST(char32_t, utf8[i]);
		}

		return buffer;
	}

	std::u32string UTF8ConvertToUTF32(const std::string_view& utf8Str)
	{
		return UTF8ConvertToUTF32(utf8Str.data());
	}

	char* UTF16ConvertToUTF8(const char16_t* utf16)
	{
		std::u16string buffer = utf16;
		const auto size = buffer.size();
		
		auto result = new char[size + 1]{ '\0' };
		for (auto i = 0u; i < size; ++i)
			result[i] = CAST(char, buffer[i]);
		return result;
	}

	std::string UTF16ConvertToUTF8(const std::u16string_view& utf16Str)
	{
		return UTF16ConvertToUTF8(utf16Str.data());
	}

	char32_t* UTF16ConvertToUTF32(const char16_t* utf16)
	{
		std::u16string buffer = utf16;
		const auto size = buffer.size();

		auto result = new char32_t[size + 1]{ U'\0' };
		for (auto i = 0u; i < size; ++i)
			result[i] = CAST(char32_t, buffer[i]);
		return result;
	}

	std::u32string UTF16ConvertToUTF32(const std::u16string_view& utf16Str)
	{
		return UTF16ConvertToUTF32(utf16Str.data());
	}

	char* UTF32ConvertToUTF8(const char32_t* utf32)
	{
		std::u32string buffer = utf32;
		const auto size = buffer.size();

		auto result = new char[size + 1]{ '\0' };
		for (auto i = 0u; i < size; ++i)
			result[i] = CAST(char, buffer[i]);
		return result;
	}

	std::string UTF32ConvertToUTF8(const std::u32string_view& utf32Str)
	{
		return UTF32ConvertToUTF8(utf32Str.data());
	}

	char16_t* UTF32ConvertToUTF16(const char32_t* utf32)
	{
		std::u32string buffer = utf32;
		const auto size = buffer.size();

		auto result = new char16_t[size + 1]{ u'\0' };
		for (auto i = 0u; i < size; ++i)
			result[i] = CAST(char16_t, buffer[i]);
		return result;
	}

	std::u16string UTF32ConvertToUTF16(const std::u32string_view& utf32Str)
	{
		return UTF32ConvertToUTF16(utf32Str.data());
	}
}
