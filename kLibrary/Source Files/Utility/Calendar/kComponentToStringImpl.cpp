#include "pch.hpp"
#include "kComponentToStringImpl.hpp"

#include "../../Type Traits/StringTraits.hpp"

#include "../String/kToString.hpp"

namespace klib::kCalendar
{
		std::string ComponentToStringImpl::operator()(const std::string_view& format, char token
			, const std::function<void(size_t, std::string&)>& matchingTokenFunc) const
		{
			std::string output;
			output.reserve(format.size());

			size_t index = 0;
			for (auto letter = format.front();
				letter != type_trait::s_NullTerminator<char>;
				letter = format[++index])
			{
				letter = kString::ToLower(letter);

				if (letter != token)
				{
					output.push_back(letter);
					continue;
				}

				const auto first = format.find_first_of(token, index);
				auto last = format.find_first_not_of(token, first);

				if (last == std::string::npos)
					last = format.size();

				const auto count = last - first;

				matchingTokenFunc(count, output);
				
				index += count - 1;

				if (index >= format.size() - 1)
					break;
			}

			return output;
		}

}
