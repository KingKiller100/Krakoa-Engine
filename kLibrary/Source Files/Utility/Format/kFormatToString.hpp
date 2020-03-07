#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <xtr1common>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4996)

namespace klib
{
	namespace kFormat
	{
		template<class CharType, typename T>
		constexpr std::basic_string<CharType> ToString( T object )
		{
			std::basic_stringstream<CharType> ss;
			ss << object;
			return ss.str();
		}

		template<typename T, typename U = T>
			constexpr 
				std::enable_if_t<std::is_same_v<std::remove_const_t<U>, std::string>, std::string>
				GetValue(const U& str)
		{
			return str;
		}

		template<typename T, typename U = T>
			constexpr 
				std::enable_if_t<std::is_same_v<std::remove_const_t<U>, std::wstring>, std::wstring>
				GetValue(const U& str)
		{
			return str;
		}

		template<typename T, typename U = T>
		constexpr 
			std::enable_if_t<std::is_signed_v<U> && !std::is_floating_point_v<U>, U>
			GetValue(const T obj)
		{
			return obj;
		}

		template<typename T, typename U = T>
		constexpr 
			std::enable_if_t<std::is_unsigned_v<U>, U>
			GetValue(const T obj)
		{
			return obj;
		}

		template<typename T, typename U = T>
		constexpr 
			std::enable_if_t<std::is_floating_point_v<U>, U>
			GetValue(const T obj)
		{
			return obj;
		}

		template<typename T, typename U = T>
		constexpr 
			std::enable_if_t<(
				!std::is_arithmetic_v<std::remove_const_t<U>> &&
				!std::is_same_v<std::remove_const_t<U>, std::string> &&
				!std::is_pointer_v<std::remove_const_t<U>> &&
				!std::is_unsigned_v<U>
				), std::string>
			GetValue(const T obj)
		{
			return obj.ToString().data();
		}

		template<typename T, typename U = T>
			std::enable_if_t<std::is_pointer_v<U> , U>
			GetValue(U obj)
		{
			return obj;
		}
  
		// Only designed for char or wide char strings
		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const CharType* format, T arg, Ts ...argPack)
		{
			using StringWriter = std::basic_string<CharType>;

			size_t length = 0;
			std::unique_ptr<CharType[]> buffer;

			auto ans = GetValue<T>(arg);

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				length = _snprintf(nullptr, 0, format, arg, argPack...) + 1;
				if (length <= 0) throw std::runtime_error("Error during char type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = std::unique_ptr<CharType[]>(new CharType[length]);
				sprintf_s(buffer.get(), length, format, arg, argPack...);
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				length = _snwprintf(nullptr, 0, format, arg, argPack...) + 1;
				if (length <= 0) throw std::runtime_error("Error during wchar_t type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = std::unique_ptr<CharType[]>(new CharType[length]);
				swprintf_s(buffer.get(), length, format, arg, argPack...);
			}
			else
				static_assert(!std::is_same_v<CharType, char> && !std::is_same_v<CharType, wchar_t> , "Can only support \"char\" and \"wchar_t\" character types");
			
			const auto formattedText = StringWriter(buffer.get(), buffer.get() + (length - 1));
			return formattedText;
		}

		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const std::basic_string<CharType>& format, T&& arg, Ts&& ...argPack)
		{
			const std::basic_string<CharType> text = ToString(format.c_str(), arg, argPack...);
			return text;
		}
	}
}

#	pragma warning(pop)
#endif