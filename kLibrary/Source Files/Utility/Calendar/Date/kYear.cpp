#include "pch.hpp"
#include "kYear.hpp"

#include "kMonth.hpp"
#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	Year Year::FromDays(const std::uint16_t days)
	{
		const auto noOfLeaps = (days / 4) / Day::DaysInYear;
		const auto years = days / Day::DaysInYear;
		const auto remainingDays = days % Day::DaysInYear;
		const auto year = (years - noOfLeaps / Day::DaysInYear)
			+ (remainingDays / Day::DaysInYear);
		return Year(year);
	}

	Year Year::FromMonths(const std::uint16_t months)
	{
		const auto year = months / Month::MonthsInYear;
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
			output.append(ToStringUsingTokenCount(count));
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
