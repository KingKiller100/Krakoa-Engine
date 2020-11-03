#pragma once

#include "kSprintf.hpp"
#include "Stringify/kStringIdentity.hpp"
#include "Stringify/kStringifyBool.hpp"
#include "Stringify/kStringifyFloatingPoint.hpp"
#include "Stringify/kStringifyInteger.hpp"
#include "Stringify/kStringifyPointer.hpp"

#include "../../Type Traits/StringTraits.hpp"
#include "../../Utility/String/kStringConverter.hpp"

#include <any>
#include <array>
#include <deque>
#include <string>
#include <variant>

namespace klib {
	namespace kString
	{
		using IdentifierPair = std::pair<std::uint16_t, std::string>;
		using IDPairQueue = std::deque<std::pair<std::uint16_t, std::string>>;

		template<typename CharType, std::size_t Size>
		IDPairQueue CreateIdentifiers(std::basic_string<CharType>& fmt, std::array<std::any, Size>& elems)
		{
			static constexpr auto openerSymbol = CharType('{');
			static constexpr auto closerSymbol = CharType('}');
			static constexpr auto precisionSymbol = CharType(':');
			static constexpr auto nullTerminator = type_trait::s_NullTerminator<CharType>;
			static constexpr auto npos = std::basic_string<CharType>::npos;

			IDPairQueue identifiers;
			for (auto openerPos = fmt.find_first_of(openerSymbol);
				openerPos != npos;
				openerPos = fmt.find_first_of(openerSymbol, openerPos + 1))
			{
				if (fmt[openerPos + 1] == openerSymbol ||
					fmt[openerPos + 1] == CharType(' ') ||
					fmt[openerPos + 1] == CharType('\t') ||
					fmt[openerPos + 1] == nullTerminator)
				{
					openerPos += 2;
					continue;
				}

				const auto closePos = fmt.find_first_of(closerSymbol, openerPos);
				const auto digits = closePos - openerPos - 1;
				const auto current = fmt.substr(openerPos + 1, digits);
				std::string bracketContents = kString::Convert<char>(current);

				const auto relativeColonPos = bracketContents.find_first_of(precisionSymbol);
				const auto optionIndex = bracketContents.substr(0, relativeColonPos);
				const auto idx = kString::StrTo<IdentifierPair::first_type>(optionIndex);
				if (elems.size() <= idx)
				{
					const auto convertedFmt = kString::Convert<char>(fmt);
					const auto errMsg = "Index given is larger than the number of objects given for string formatting\n"
						"Please check your format again: " + convertedFmt + "\n" "Index: " + optionIndex;
					throw std::out_of_range(errMsg);
				}

				const auto type = elems[idx].type().name();

				identifiers.push_back(std::make_pair(idx, type));
			}
			identifiers.shrink_to_fit();

			return identifiers;
		}

		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const std::basic_string_view<CharType>& format, T arg, Ts ...argPack)
		{
			using namespace kString;
			using DataTypes = std::variant<std::monostate, T, Ts...>;

			static constexpr auto printfSymbol = CharType('%');
			static constexpr auto openerSymbol = CharType('{');
			static constexpr auto closerSymbol = CharType('}');
			static constexpr auto precisionSymbol = CharType(':');
			static constexpr auto nullTerminator = type_trait::s_NullTerminator<CharType>;
			static constexpr auto npos = std::basic_string<CharType>::npos;

			if (auto pfSymPos = format.find(printfSymbol); pfSymPos != npos)
			{
				return stringify::Sprintf<CharType>(format, arg, argPack...);
			}

			std::array<std::any, std::variant_size_v<DataTypes> -1> elems = { stringify::IdentityPtr<CharType, T>(arg)
				, stringify::IdentityPtr<CharType, Ts>(argPack)... };

			std::basic_string<CharType> fmt(format);
			IDPairQueue identifiers = CreateIdentifiers(fmt, elems);

			std::basic_string<CharType> finalString;
			for (const auto& id : identifiers)
			{
				const auto& val = elems[id.first];
				const auto& type = id.second;
				const auto inputPos = fmt.find_first_of(closerSymbol) + 1;
				auto currentSection = fmt.substr(0, inputPos);
				auto replacePos = currentSection.find_first_of(openerSymbol);
				auto colonPos = currentSection.find(precisionSymbol, replacePos);
				size_t padding = stringify::nPrecision;

				if (colonPos != npos)
				{
					padding = std::stoll(Convert<char>(currentSection.substr(colonPos + 1, inputPos - 1)));
				}

				currentSection.erase(replacePos);

				if (Count(type, '*') > 1)
				{
					if (Contains(type, "char"))
					{
						const auto data = std::any_cast<const CharType* const*>(val);
						currentSection.append(*data);
						finalString.append(currentSection);
					}
					else
					{
						auto data = std::any_cast<const void* const*>(val);
						currentSection.append(stringify::StringifyPointer<CharType>(*data, padding));
						finalString.append(currentSection);
					}
				}
				else if (Contains(type, "basic_string_view"))
				{
					const auto data = std::any_cast<const std::basic_string_view<CharType>*>(val);
					currentSection.append(*data);
					finalString.append(currentSection);
				}
				else if (Contains(type, "basic_string"))
				{
					const auto data = std::any_cast<const std::basic_string<CharType>*>(val);
					currentSection.append(*data);
					finalString.append(currentSection);
				}
				else if (Contains(type, "unsigned"))
				{
					if (Contains(type, "char"))
					{
						auto data = std::any_cast<const unsigned char*>(val);
						currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
						finalString.append(currentSection);
					}
					else if (Contains(type, "short"))
					{
						auto data = std::any_cast<const unsigned short*>(val);
						currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
						finalString.append(currentSection);
					}
					else if (Contains(type, "int"))
					{
						if (Contains(type, "__int64"))
						{
							auto data = std::any_cast<const unsigned __int64*>(val);
							currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
							finalString.append(currentSection);
						}
						else
						{
							auto data = std::any_cast<const unsigned int*>(val);
							currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
							finalString.append(currentSection);
						}
					}
					else if (Contains(type, "long"))
					{
						if (Contains(type, "long long"))
						{
							auto data = std::any_cast<const unsigned long long*>(val);
							currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
							finalString.append(currentSection);
						}
						else
						{
							auto data = std::any_cast<const unsigned long*>(val);
							currentSection.append(stringify::StringUnsignedIntegral<CharType>(*data, padding));
							finalString.append(currentSection);
						}
					}
				}
				else if (Contains(type, "long"))
				{
					if (Contains(type, "double"))
					{
						auto data = std::any_cast<const long double*>(val);
						currentSection += stringify::StringFloatingPoint<CharType>(*data, padding);
						finalString.append(currentSection);
					}
					else if (Contains(type, "int"))
					{
						auto data = std::any_cast<const long int*>(val);
						currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
						finalString.append(currentSection);
					}
					else if (Contains(type, "long long"))
					{
						const auto data = std::any_cast<const long long*>(val);
						currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
						finalString.append(currentSection);
					}
					else
					{
						auto data = std::any_cast<const long*>(val);
						currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
						finalString.append(currentSection);
					}
				}
				else if (Contains(type, "char"))
				{
					const auto data = std::any_cast<const CharType*>(val);
					currentSection.push_back(*data);
					finalString.append(currentSection);
				}
				else if (Contains(type, "short"))
				{
					auto data = std::any_cast<const short*>(val);
					currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
					finalString.append(currentSection);
				}
				else if (Contains(type, "int"))
				{
					if (Contains(type, "__int64"))
					{
						auto data = std::any_cast<const __int64*>(val);
						currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
						finalString.append(currentSection);
					}
					else
					{
						auto data = std::any_cast<const int*>(val);
						currentSection += stringify::StringSignedIntegral<CharType>(*data, padding);
						finalString.append(currentSection);
					}
				}
				else if (Contains(type, "double"))
				{
					const auto data = std::any_cast<const double*>(val);
					currentSection += stringify::StringFloatingPoint<CharType>(*data, padding);
					finalString.append(currentSection);
				}
				else if (Contains(type, "float"))
				{
					const auto data = std::any_cast<const float*>(val);
					currentSection.append(stringify::StringFloatingPoint<CharType>(*data, padding));
					finalString.append(currentSection);
				}
				else if (Contains(type, "bool"))
				{
					const auto res = std::any_cast<const bool*>(val);
					currentSection.append(stringify::StringBool<CharType>(*res));
					finalString.append(currentSection);
				}
				else
				{
					throw std::runtime_error("Type entered not recognised/supported");
				}

				fmt.erase(0, inputPos);
				identifiers.pop_front();
			}

			if (!fmt.empty())
				finalString.append(fmt);

			return finalString;
		}

		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const std::basic_string<CharType>& format, const T& arg, const Ts& ...argPack)
		{
			const std::basic_string_view<CharType> fmt(format);
			const std::basic_string<CharType> text = ToString(fmt, arg, argPack...);
			return text;
		}

		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const CharType* format, const T& arg, const Ts& ...argPack)
		{
			const std::basic_string_view<CharType> fmt(format);
			const std::basic_string<CharType> text = ToString(fmt, arg, argPack...);
			return text;
		}

		template<class CharType, typename T>
		constexpr std::basic_string<CharType> ToString(T&& object)
		{
			const auto fmt = kString::Convert<CharType>("{0}");
			const auto s = ToString(fmt, object);
			return s;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}
