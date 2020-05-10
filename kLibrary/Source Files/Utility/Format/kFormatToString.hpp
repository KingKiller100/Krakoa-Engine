#pragma once

#include "../../Type Traits/String.hpp"
#include "../../Utility/String/kUTFStringConverter.hpp"

#include <any>
#include <array>
#include <cstdio>
#include <deque>
#include <string>
#include <sstream>
#include <variant>
#include <vector>
#include <xtr1common>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4996)

namespace klib
{
	namespace kFormat
	{
		// C++ STL string/string_view
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <type_trait::Is_CharType_V<C>
			&& type_trait::Is_StringType_V<U>,
			const typename T::value_type*>
			GetValue(const T& str)
		{
			return str.data();
		}

		// Non C string, but primative type ptrs
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t<type_trait::Is_CharType_V<C>
			&& std::is_pointer_v<U>
			&& !type_trait::Is_CharType_V<ONLY_TYPE(U)>
			&& !type_trait::Is_StringType_V<ONLY_TYPE(U)>
			, const void*>
			GetValue(const T obj)
		{
			return (const void*)obj;
		}

		// C string ptrs
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t<type_trait::Is_CharType_V<C>
			&& std::is_pointer_v<U>
			&& type_trait::Is_CharType_V<ONLY_TYPE(U)>
			&& !type_trait::Is_StringType_V<ONLY_TYPE(U)>
			, const T>
			GetValue(const T obj)
		{
			return obj;
		}

		// Primative types (int, double, unsigned long long,...) but not bool
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t<type_trait::Is_CharType_V<C>
			&& std::is_arithmetic_v<U>
			, const U>
			GetValue(const T obj)
		{
			return obj;
		}

		// Non-primative types
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t<(type_trait::Is_CharType_V<C>
				&& !std::is_arithmetic_v<std::decay_t<U>>
				&& !type_trait::Is_StringType_V<U>
				&& !std::is_pointer_v<std::decay_t<U>>
				), const CharType*>
			GetValue(const T& obj)
		{
			return obj.ToString().data();
		}

		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <type_trait::Is_CharType_V<C>
			&& type_trait::Is_StringType_V<U>,
			const T*>
			GetValuePtr(const T& str)
		{
			return &str;
		}

		// Non C string, but primative ptrs
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <type_trait::Is_CharType_V<C>
			&& std::is_pointer_v<U>
			&& !type_trait::Is_CharType_V<ONLY_TYPE(U)>
			&& !type_trait::Is_StringType_V<ONLY_TYPE(U)>
			, const void*>
			GetValuePtr(const T obj)
		{
			return (const void*)obj;
		}

		// C string ptrs
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <type_trait::Is_CharType_V<C>
			&& std::is_pointer_v<U>
			&& type_trait::Is_CharType_V<ONLY_TYPE(U)>
			&& !type_trait::Is_StringType_V<ONLY_TYPE(U)>
			, const T>
			GetValuePtr(const T obj)
		{
			return obj;
		}

		// primative types (int, double, unsigned long long,...)
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <type_trait::Is_CharType_V<C>
			&& std::is_arithmetic_v<U>
			, const U*>
			GetValuePtr(const T& obj)
		{
			return &obj;
		}

		// Non-primative custom types - Must have a function ToString that returns a C++ STL string type
		template<typename CharType, typename T, typename C = CharType, typename U = T>
		constexpr
			std::enable_if_t <(type_trait::Is_CharType_V<C>
				&& !std::is_arithmetic_v<std::decay_t<U>>
				&& !type_trait::Is_StringType_V<U>
				&& !std::is_pointer_v<std::decay_t<U>>
				), const std::basic_string<C>*>
			GetValuePtr(T obj)
		{
			static std::basic_string<CharType> str;
			str = obj.ToString();
			return &str;
		}

		template<typename CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> MakeStringFromData(const std::basic_string<CharType>& format, T arg1, Ts ...argN)
		{
			CharType* buffer;
			int length = -1;

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				length = _snprintf(nullptr, 0, format.data(), arg1, argN...) + 1;
				if (length <= 0) throw std::runtime_error("Error during char type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = new CharType[length]();
				sprintf_s(buffer, length, format.data(), arg1, argN...);
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				length = _snwprintf(nullptr, 0, format.data(), arg1, argN...) + 1;
				if (length <= 0) throw std::runtime_error("Error during wchar_t type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = new CharType[length]();
				swprintf_s(buffer, length, format.data(), arg1, argN...);
			}
			else
			{
				auto str = kString::Convert<char>(format);
				str = MakeStringFromData<char>(str.data(), arg1, argN...);
				const auto text = kString::Convert<CharType>(str);

				return text;
			}

			const auto formattedText = std::basic_string<CharType>(buffer, buffer + (CAST(ptrdiff_t, length) - 1));
			delete[] buffer;
			return formattedText;
		}

		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharType, typename T, typename ...Ts>
		constexpr inline std::basic_string<CharType> ToString(std::basic_string<CharType>& format, const T arg, const Ts ...argPack)
		{
			using DataTypes = std::variant<std::monostate, T, Ts...>;

			static constexpr auto printfSymbol = CharType('%');
			static constexpr auto openerSymbol = CharType('{');
			static constexpr auto closerSymbol = CharType('}');

			static constexpr auto npos = std::basic_string<CharType>::npos;

			if (auto pfSymPos = format.find(printfSymbol); pfSymPos != npos)
			{
				return MakeStringFromData<CharType>(format, GetValue<CharType, T>(arg), GetValue<CharType, Ts>(argPack)...);
			}

			std::array<std::any, std::variant_size_v<DataTypes> -1> elems = { GetValuePtr<CharType, T>(arg), GetValuePtr<CharType, Ts>(argPack)... };

			std::deque<std::pair<unsigned char, std::string>> identifiers;
			for (auto i = format.find_first_of(openerSymbol); i != npos; i = format.find_first_of(openerSymbol, i + 1))
			{
				if (format[i + 1] == format[i] ||
					format[i + 1] == CharType(' ') ||
					format[i + 1] == CharType('\t') ||
					format[i + 1] == CharType('\0'))
				{
					i += 2;
					continue;
				}

				std::string objIndex;

				const auto closePos = format.find_first_of(closerSymbol, i);
				for (auto j = 1; (i + j) < closePos; ++j)
					objIndex += (char)format[i + j];

				const auto idx = (unsigned char)std::stoi(objIndex);
				identifiers.push_back(std::make_pair(idx, elems[idx].type().name()));
			}
			identifiers.shrink_to_fit();

			std::basic_string<CharType> finalString;
			for (const auto& id : identifiers)
			{
				auto& val = elems[id.first];
				const auto inputPos = format.find_first_of(closerSymbol) + 1;
				auto currentSection = format.substr(0, inputPos);
				auto replacePos = currentSection.find_first_of(openerSymbol);
				auto colonPos = currentSection.find(CharType(':'), replacePos);
				auto padding = CharType('\0');
				if (colonPos != npos)
				{
					padding = currentSection[colonPos + 1];
				}
				currentSection[replacePos] = printfSymbol;
				currentSection.erase(replacePos + 2);

				if (padding != CharType('\0'))
				{
					currentSection.insert(replacePos + 1, 1, padding);
					currentSection.insert(replacePos + 1, 1, CharType('0'));
					replacePos += 2;
				}

				if (id.second.find("void") != npos)
				{
					currentSection[replacePos + 1] = CharType('p');
					auto data = std::any_cast<const void*>(val);
					finalString.append(MakeStringFromData(currentSection, data));
				}
				else if (id.second.find("basic_string_view") != npos)
				{
					const auto data = std::any_cast<const std::basic_string_view<CharType>*>(val);
					currentSection.erase(replacePos);
					currentSection.insert(replacePos, data->data());
					finalString.append(currentSection);
				}
				else if (id.second.find("basic_string") != npos)
				{
					const auto data = std::any_cast<const std::basic_string<CharType>*>(val);
					currentSection.erase(replacePos);
					currentSection.insert(replacePos, data->data());
					finalString.append(currentSection);
				}
				else if (id.second.find("char") != npos)
				{
					const auto data = std::any_cast<const CharType*>(val);
					currentSection.erase(replacePos);
					currentSection.insert(replacePos, data);
					finalString.append(currentSection);
				}
				else if (id.second.find("unsigned") != npos)
				{
					currentSection[replacePos + 1] = CharType('u');

					if (id.second.find("char") != npos)
					{
						auto data = std::any_cast<const unsigned char*>(val);
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else if (id.second.find("short") != npos)
					{
						auto data = std::any_cast<const unsigned short*>(val);
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else if (id.second.find("int") != npos)
					{
						auto data = std::any_cast<const unsigned int*>(val);
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else if (auto longPos = id.second.find("long"); longPos != npos)
					{
						auto long2Pos = id.second.find_first_of("long long");
						if (long2Pos != npos)
						{
							auto data = std::any_cast<const unsigned long long*>(val);
							currentSection.insert(replacePos + 1, 1, CharType('l'));
							finalString.append(MakeStringFromData(currentSection, *data));
						}
						else
						{
							auto data = std::any_cast<const unsigned long*>(val);
							currentSection.insert(replacePos + 1, 2, CharType('l'));
							finalString.append(MakeStringFromData(currentSection, *data));
						}
					}
				}
				else if (const auto longPos = id.second.find("long"); longPos != npos)
				{
					if (id.second.find_first_of("double", longPos + 4) != npos)
					{
						auto data = std::any_cast<const long double*>(val);
						currentSection[replacePos + 1] = CharType('f');
						if (colonPos != npos)
							currentSection[replacePos - 1] = CharType('.');
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else if (id.second.find_first_of("int", longPos + 4) != npos)
					{
						auto data = std::any_cast<const long int*>(val);
						currentSection[replacePos + 1] = CharType('l');
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else if (const auto long2Pos = id.second.find_first_of("long long"); long2Pos != npos)
					{
						const auto data = std::any_cast<const long long*>(val);
						currentSection.insert(replacePos + 1, 2, CharType('l'));
						finalString.append(MakeStringFromData(currentSection, *data));
					}
					else
					{
						auto data = std::any_cast<const long*>(val);
						currentSection[replacePos + 1] = CharType('l');
						finalString.append(MakeStringFromData(currentSection, *data));
					}
				}
				else if (id.second.find("short") != npos)
				{
					auto data = std::any_cast<const short*>(val);
					currentSection[replacePos + 1] = CharType('d');
					finalString.append(MakeStringFromData(currentSection, *data));
				}
				else if (id.second.find("int") != npos)
				{
					auto data = std::any_cast<const int*>(val);
					currentSection[replacePos + 1] = CharType('d');
					finalString.append(MakeStringFromData(currentSection, *data));
				}
				else if (id.second.find("double") != npos)
				{
					const auto data = std::any_cast<const double*>(val);
					currentSection[replacePos + 1] = CharType('f');
					if (colonPos != npos)
						currentSection[replacePos - 1] = CharType('.');
					finalString.append(MakeStringFromData(currentSection, *data));
				}
				else if (id.second.find("float") != npos)
				{
					const auto data = std::any_cast<const float*>(val);
					currentSection[replacePos + 1] = CharType('f');
					if (colonPos != npos)
						currentSection[replacePos - 1] = CharType('.');
					finalString.append(MakeStringFromData(currentSection, *data));
				}
				else if (id.second.find("bool") != npos)
				{
					const auto res = std::any_cast<const bool*>(val);
					std::basic_string_view<CharType> data;
					if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
						data = *res ? "true" : "false";
					else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
						data = *res ? L"true" : L"false";
					else if _CONSTEXPR_IF(std::is_same_v<CharType, char16_t>)
						data = *res ? u"true" : u"false";
					else if _CONSTEXPR_IF(std::is_same_v<CharType, char32_t>)
						data = *res ? U"true" : U"false";
#ifdef __cpp_char8_t
					else if _CONSTEXPR_IF(std::is_same_v<CharType, char8_t>)
						data = *res ? u8"true" : u8"false";
#endif
					currentSection.erase(replacePos);
					currentSection.insert(replacePos, data);

					finalString.append(currentSection);
				}
				else
				{
					throw std::runtime_error("Type entered not recognised/supported");
				}

				format.erase(0, inputPos);
				identifiers.pop_front();
			}

			if (!format.empty())
				finalString.append(format);

			return finalString;
		}

		template<class CharType, typename T, typename ...Ts>
		constexpr inline std::basic_string<CharType> ToString(const CharType* format, const T arg, const Ts ...argPack)
		{
			std::basic_string<CharType> formatStr = format;
			const std::basic_string<CharType> text = ToString(formatStr, arg, argPack...);
			return text;
		}

		template<class CharType, typename T>
		constexpr std::basic_string<CharType> ToString(T object)
		{
			std::basic_stringstream<CharType> ss;
			ss << object;
			return ss.str();
		}
	}
}

#	pragma warning(pop)
#endif