#pragma once

#include "../HelperMacros.hpp"

#include <string>
#include <xtr1common>

namespace klib::type_trait
{
	template<typename T>
	struct Is_CharTypeBase : std::false_type
	{};

	template<>
	struct Is_CharTypeBase<char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<unsigned char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<signed char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<wchar_t> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<char16_t> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<char32_t> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_CharTypeBase<char8_t> : std::true_type
	{};
#endif

	template<typename T>
	struct Is_CharType : Is_CharTypeBase<std::remove_cv_t<T>>
	{};

	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::value;

	template<typename T>
	struct Is_StringTypeBase : std::false_type
	{};

	template<>
	struct Is_StringTypeBase<std::string> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::wstring> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::u16string> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::u32string> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringTypeBase<std::u8string> : std::true_type
	{};
#endif

	template<>
	struct Is_StringTypeBase<std::string_view> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::wstring_view> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::u16string_view> : std::true_type
	{};

	template<>
	struct Is_StringTypeBase<std::u32string_view> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringTypeBase<std::u8string_view> : std::true_type
	{};
#endif

	template<typename T>
	struct Is_StringType : Is_StringTypeBase<std::remove_cv_t<T>>
	{};

	template<typename T>
	constexpr bool Is_StringType_V = Is_StringType<T>::value;

	template<typename CharType, typename = std::enable_if_t<Is_CharType_V<CharType>>>
	constexpr auto nullTerminator = CharType();
}