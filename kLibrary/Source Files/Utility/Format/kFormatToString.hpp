#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>
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
				std::enable_if_t<std::is_same_v<U, std::string>, const char*>
				GetValue(const U && str)
		{
			return str.data();
		}

		template<typename T, typename U = T>
		constexpr std::enable_if_t<
			std::is_arithmetic_v<U>, U>
			GetValue(const T str)
		{
			return str;
		}

		// Only designed for char or wide char strings
		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const CharType* format, T arg, Ts ...argPack)
		{
			size_t length = 0;
			std::unique_ptr<CharType[]> buffer;

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
				throw std::runtime_error("Can only support \"char\" and \"wchar_t\" character types");
			
			const auto formattedText = std::basic_string<CharType>(buffer.get(), buffer.get() + (length - 1));
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