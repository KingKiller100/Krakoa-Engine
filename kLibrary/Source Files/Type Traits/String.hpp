#pragma once

#include "../HelperMacros.hpp"

#include <string>
#include <xtr1common>

namespace klib::type_trait
{
	template<typename T>
	struct Is_CharType : std::false_type
	{};

	template<>
	struct Is_CharType<char> : std::true_type
	{};

	template<>
	struct Is_CharType<unsigned char> : std::true_type
	{};

	template<>
	struct Is_CharType<signed char> : std::true_type
	{};

	template<>
	struct Is_CharType<wchar_t> : std::true_type
	{};

	template<>
	struct Is_CharType<char16_t> : std::true_type
	{};

	template<>
	struct Is_CharType<char32_t> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_CharType<char8_t> : std::true_type
	{};
#endif

	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::value;

	template<typename T>
	struct Is_StringType : std::false_type
	{};

	template<>
	struct Is_StringType<std::string> : std::true_type
	{};

	template<>
	struct Is_StringType<std::wstring> : std::true_type
	{};

	template<>
	struct Is_StringType<std::u16string> : std::true_type
	{};

	template<>
	struct Is_StringType<std::u32string> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringType<std::u8string> : std::true_type
	{};
#endif

	template<>
	struct Is_StringType<std::string_view> : std::true_type
	{};

	template<>
	struct Is_StringType<std::wstring_view> : std::true_type
	{};

	template<>
	struct Is_StringType<std::u16string_view> : std::true_type
	{};

	template<>
	struct Is_StringType<std::u32string_view> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringType<std::u8string_view> : std::true_type
	{};
#endif

	template<typename T>
	constexpr bool Is_StringType_V = Is_StringType<T>::value;
}