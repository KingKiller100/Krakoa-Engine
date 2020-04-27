#pragma once

#include "../../HelperMacros.hpp"

#include <string>

namespace klib::type_trait
{
	template<bool Val, typename T>
	struct BooleanConstant
	{
	public:
		using Type = T;
		static constexpr bool Value = Val;

		[[nodiscard]] constexpr static bool GetValue() noexcept
		{
			return Val;
		}
	};

	template<typename T>
	struct TrueConstant : BooleanConstant<true, T>
	{};

	template<typename T>
	struct FalseConstant : BooleanConstant<false, T>
	{};

	template<typename CharType>
	struct Is_CharType : FalseConstant<CharType>
	{};

	template<>
	struct Is_CharType<char> : TrueConstant<char>
	{};

	template<>
	struct Is_CharType<unsigned char> : TrueConstant<unsigned char>
	{};

	template<>
	struct Is_CharType<signed char> : TrueConstant<signed char>
	{};

	template<>
	struct Is_CharType<wchar_t> : TrueConstant<wchar_t>
	{};

	template<>
	struct Is_CharType<char16_t> : TrueConstant<char16_t>
	{};

	template<>
	struct Is_CharType<char32_t> : TrueConstant<char32_t>
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_CharType<char8_t> : TrueConstant<char8_t>
	{};
#endif

	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::Value;


	template<typename T>
	struct Is_StringType : FalseConstant<T>
	{};

	template<>
	struct Is_StringType<std::string> : TrueConstant<std::string>
	{};

	template<>
	struct Is_StringType<std::wstring> : TrueConstant<std::wstring>
	{};

	template<>
	struct Is_StringType<std::u16string> : TrueConstant<std::u16string>
	{};

	template<>
	struct Is_StringType<std::u32string> : TrueConstant<std::u32string>
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringType<std::u8string> : TrueConstant<std::u8string>
	{};
#endif

	template<>
	struct Is_StringType<std::string_view> : TrueConstant<std::string_view>
	{};

	template<>
	struct Is_StringType<std::wstring_view> : TrueConstant<std::wstring_view>
	{};

	template<>
	struct Is_StringType<std::u16string_view> : TrueConstant<std::u16string_view>
	{};

	template<>
	struct Is_StringType<std::u32string_view> : TrueConstant<std::u32string_view>
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_StringType<std::u8string_view> : TrueConstant<std::u8string_view>
	{};
#endif


	template<typename T>
	constexpr bool Is_StringType_V = Is_StringType<T>::Value;

}


