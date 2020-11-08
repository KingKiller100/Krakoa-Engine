#include "pch.hpp"
#include "kComponentToStringImpl.hpp"

#include "../../Type Traits/StringTraits.hpp"

#include "../String/kToString.hpp"

namespace klib::kCalendar
{
		void CalendarComponentToStringImpl::ToStringImpl(const std::string_view& format, char token
			, const std::function<void(char)>& noMatchingTokenFunc
			, const std::function<void(size_t)>& matchingTokenFunc) const
		{
			size_t index = 0;
			for (auto letter = format.front();
				letter != type_trait::s_NullTerminator<char>;
				letter = format[++index])
			{
				letter = kString::ToLower(letter);

				if (letter != token)
				{
					noMatchingTokenFunc(letter);
					continue;
				}

				const auto first = format.find_first_of(token, index);
				auto last = format.find_first_not_of(token, first);

				if (last == std::string::npos)
					last = format.size();

				const auto count = last - first;

				matchingTokenFunc(count);
				
				index += count - 1;

				if (index >= format.size() - 1)
					break;
			}
		}

}
