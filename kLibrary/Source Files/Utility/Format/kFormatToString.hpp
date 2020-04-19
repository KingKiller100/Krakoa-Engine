#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <xtr1common>

#include <any>
#include <array>
#include <deque>
#include <variant>
#include <vector>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4996)

namespace klib
{
	namespace kFormat
	{
		template<class CharType, typename T>
		constexpr std::basic_string<CharType> ToString(T object)
		{
			std::basic_stringstream<CharType> ss;
			ss << object;
			return ss.str();
		}

		template<typename T, typename C = std::basic_string<ONLY_TYPE(T)>, typename U = T>
		constexpr
			std::enable_if_t < std::is_same_v < U, std::basic_string<typename U::value_type>>, const typename T::value_type*>
			GetValue(const C& str)
		{
			return str.data();
		}

		template<typename T, typename C = std::basic_string_view<ONLY_TYPE(T)>, typename U = T>
		constexpr
			std::enable_if_t < std::is_same_v < U, std::basic_string_view<typename U::value_type>>, const typename T::value_type*>
			GetValue(const C& str)
		{
			return str.data();
		}

		template<typename T, typename U = T>
		constexpr
			std::enable_if_t<std::is_arithmetic_v<U>
			|| std::is_pointer_v<U>
			, U>
			GetValue(const T obj)
		{
			return obj;
		}

		template<typename T, typename C = std::basic_string<ONLY_TYPE(T)>, typename U = T>
		constexpr
			std::enable_if_t<(
				!std::is_arithmetic_v<std::decay_t<U>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string<char>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string<wchar_t>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string<char16_t>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string<char32_t>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string_view<char>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string_view<wchar_t>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string_view<char16_t>> &&
				!std::is_same_v<std::decay_t<U>, std::basic_string_view<char32_t>> &&
				!std::is_pointer_v<std::decay_t<U>> &&
				!std::is_unsigned_v<std::decay_t<U>>
				), const typename T::value_type*>
			GetValue(const T obj)
		{
			return obj.ToString().data();
		}

		template<typename CharType, typename T, typename ...Ts>
		constexpr inline std::basic_string<CharType> MakeStringFromData(const std::basic_string<CharType>& format, T data, Ts ...params)
		{
			size_t length = 0;
			CharType* buffer;

			if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			{
				length = _snprintf(nullptr, 0, format.data(), data, params...) + 1;
				if (length <= 0) throw std::runtime_error("Error during char type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = new CharType[length]();
				sprintf_s(buffer, length, format.data(), data, params...);
			}
			else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			{
				length = _snwprintf(nullptr, 0, format.data(), data, params...) + 1;
				if (length <= 0) throw std::runtime_error("Error during wchar_t type \"ToString(...)\" formatting: string returned length <= 0");
				buffer = new CharType[length]();
				swprintf_s(buffer, length, format.data(), data, params...);
			}
			else
			{
				std::string str;
				for (auto& c : format)
					str += (char)c;
				str = MakeStringFromData<char>(str.data(), data, params...);
				std::basic_string<CharType> text;
				for (auto& c : str)
				{
					text += c;
				}
				return text;
			}

			const auto formattedText = std::basic_string<CharType>(buffer, buffer + (length - 1));
			delete[] buffer;
			return formattedText;
		}

		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharType, typename T, typename ...Ts>
		constexpr inline std::basic_string<CharType> ToString(const CharType* format, T arg, Ts ...argPack)
		{
			using ParseTypes = std::variant<std::monostate, T, Ts...>;

			constexpr static auto printfSymbol = CharType('%');
			constexpr static auto openerSymbol = CharType('{');
			constexpr static auto closerSymbol = CharType('}');

			constexpr static auto npos = std::basic_string<CharType>::npos;

			auto text = std::basic_string<CharType>(format);

			if (text.find(printfSymbol) != npos)
				return MakeStringFromData<CharType>(format, GetValue<T>(arg), GetValue<Ts>(argPack)...);

			std::basic_string<CharType> res;
			std::array<std::any, std::variant_size_v<ParseTypes> -1> elems = { GetValue<T>(arg), GetValue<Ts>(argPack)... };

			std::deque<std::pair<short, std::string>> identifiers;
			for (auto i = text.find_first_of(openerSymbol); i != npos; i = text.find_first_of(openerSymbol, i + 1))
			{
				if (text[i + 1] == text[i] ||
					text[i + 1] == CharType(' ') ||
					text[i + 1] == CharType('\t') ||
					text[i + 1] == CharType('\0'))
				{
					i += 2;
					continue;
				}

				std::string objIndex;

				const auto closePos = text.find_first_of(closerSymbol, i);
				for (auto j = 1; (i + j) < closePos; ++j)
				{
					objIndex += (char)text[i + j];
				}

				const short idx = (short)std::stoi(objIndex);

				identifiers.push_back(std::make_pair(idx, elems[idx].type().name()));
			}
			identifiers.shrink_to_fit();

			for (const auto& id : identifiers)
			{
				auto& val = elems[id.first];
				const auto inputPos = text.find_first_of(closerSymbol) + 1;
				auto sub = text.substr(0, inputPos);
				auto colonPos = sub.find(CharType(':'));
				CharType padding;
				if (colonPos != npos)
				{
					padding = sub[colonPos + 1];
				}
				auto replacePos = sub.find_first_of(openerSymbol);
				sub[replacePos] = CharType('%');
				sub.erase(replacePos + 2);

				if (colonPos != npos)
				{
					sub.insert(replacePos + 1, 1, padding);
					replacePos++;
				}

				if (id.second.find(CharType('*')) != npos)
				{
					if (id.second.find("char") != npos)
					{
						sub[replacePos + 1] = CharType('s');
						auto data = std::any_cast<const CharType*>(val);
						res.append(MakeStringFromData(sub, data));
					}
					else
					{
						throw std::runtime_error("Non-char type pointer not recognised/supported");
						//sub[replacePos + 1] = CharType('p');
						//res.append(MakeStringFromData(sub, data));
					}
				}
				else if (id.second.find("unsigned") != npos)
				{
					unsigned long long data;
					sub[replacePos + 1] = CharType('u');

					if (id.second.find("char") != npos)
					{
						data = (unsigned long long)std::any_cast<unsigned char>(val);
					}
					else if (id.second.find("int") != npos)
					{
						data = (unsigned long long)std::any_cast<unsigned int>(val);
					}
					else if (auto longPos = id.second.find("long"); longPos != npos)
					{
						auto long2Pos = id.second.find_first_of("long", longPos + 4);
						if (long2Pos != npos)
						{
							data = std::any_cast<unsigned long long>(val);
							sub.insert(replacePos + 1, 1, CharType('l'));
						}
						else
						{
							data = (unsigned long long)std::any_cast<unsigned long>(val);
							sub.insert(replacePos + 1, 2, CharType('l'));
						}
					}

					res.append(MakeStringFromData(sub, data));
				}
				else if (auto longPos = id.second.find("long"); longPos != npos)
				{
					auto long2Pos = id.second.find_first_of("long", longPos + 4);
					if (long2Pos != npos)
					{
						auto data = std::any_cast<long long>(val);
						sub.insert(replacePos + 1, 2, CharType('l'));
						res.append(MakeStringFromData(sub, data));
					}
					else if (id.second.find_first_of("double", longPos + 4) != npos)
					{
						auto data = std::any_cast<long double>(val);
						sub[replacePos + 1] = CharType('f');
						res.append(MakeStringFromData(sub, data));
					}
					else if (id.second.find_first_of("int", longPos + 4) != npos)
					{
						auto data = std::any_cast<long int>(val);
						sub[replacePos + 1] = CharType('l');
						res.append(MakeStringFromData(sub, data));
					}
					else
					{
						auto data = std::any_cast<long>(val);
						sub[replacePos + 1] = CharType('l');
						res.append(MakeStringFromData(sub, data));
					}
				}
				else if (id.second.find("short") != npos)
				{
					auto data = std::any_cast<short>(val);
					sub[replacePos + 1] = CharType('d');
					res.append(MakeStringFromData(sub, data));
				}
				else if (id.second.find("int") != npos)
				{
					auto data = std::any_cast<int>(val);
					sub[replacePos + 1] = CharType('d');
					res.append(MakeStringFromData(sub, data));
				}
				else if (id.second.find("double") != npos)
				{
					auto data = std::any_cast<double>(val);
					sub[replacePos + 1] = CharType('f');
					if (colonPos != npos)
					{
						sub.insert(replacePos, 1, CharType('.'));
					}
					res.append(MakeStringFromData(sub, data));
				}
				else if (id.second.find("float") != npos)
				{
					auto data = std::any_cast<float>(val);
					sub[replacePos + 1] = CharType('f');
					if (colonPos != npos)
					{
						sub.insert(replacePos, 1, CharType('.'));
					}
					res.append(MakeStringFromData(sub, data));
				}
				else
				{
					throw std::runtime_error("Type entered not recognised/supported");
				}

				text.erase(0, inputPos);
				identifiers.pop_front();
			}

			return res;
		}

		template<class CharType, typename T, typename ...Ts>
		constexpr inline std::basic_string<CharType> ToString(const std::basic_string<CharType>& format, T&& arg, Ts&& ...argPack)
		{
			const std::basic_string<CharType> text = ToString(format.c_str(), arg, argPack...);
			return text;
		}
	}
}

#	pragma warning(pop)
#endif