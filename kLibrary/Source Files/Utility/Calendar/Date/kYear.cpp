#include "pch.hpp"
#include "kYear.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Year::GetYearStr() const
	{
		return kString::ToString<char>(year);
	}

	std::string Year::ToString(const std::string_view& format) const
	{
		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count)
		{
			std::string toAppend;
			if (count <= 2)
				toAppend = GetYearStr().substr(2);
			else
				toAppend = GetYearStr();
			output.append(toAppend);
		};

		ToStringImpl(format, 'y', noMatchFunc, matchFunc);
		return output;
	}
}
