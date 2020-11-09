#include "pch.hpp"
#include "kYear.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	Year Year::FromDays( const std::uint16_t days )
	{
		const auto noOfLeaps = (days / 4) / daysInYear;
		const auto years = days / daysInYear;
		const auto remainingDays = days % daysInYear;
		const auto year = (years - noOfLeaps / daysInYear) + (remainingDays / daysInYear);
		return Year(year);
	}

	Year Year::FromMonths( const std::uint16_t months )
	{
		const auto year = months / 12;
		return Year(year);
	}

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
			output.append(ToStringUsingTokenCount( count ));
		};

		ToStringImpl(format, 'y', noMatchFunc, matchFunc);
		return output;
	}

	std::string Year::ToStringUsingTokenCount(const size_t count) const
	{
		if (count <= 2)
			return GetYearStr().substr(2);

		return  GetYearStr();
	}
}
