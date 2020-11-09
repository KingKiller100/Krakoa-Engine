#include <pch.hpp>

#include "kDate.hpp"

#include <algorithm>

#include "../kiCalendarInfoSource.hpp"

#include "../../../Type Traits/StringTraits.hpp"

#include "../../Debug Helper/Exceptions/CalenderExceptions.hpp"


#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	namespace
	{
		using namespace secret::helper;

		iCalendarInfoSource& GetInfoSource(CalendarInfoSourceType type)
		{
			static iCalendarInfoSource& calendar_info = GetCalendarInfoSource();
			calendar_info.Refresh(type);
			return calendar_info;
		}
	}

	// DATE ////////////////////////////////////////////////////////////

	Date::Date(CalendarInfoSourceType sourceType)
		: Date(GetInfoSource(sourceType))
	{}

	Date::Date(const iCalendarInfoSource& source)
		: Date(
			static_cast<Day::DayOfTheWeek>(source.GetDayOfTheWeekIndex())
			, day.GetDay()
			, static_cast<Month::MonthOfTheYear>(source.GetMonth())
			, source.GetYear()
		)
	{}

	Date::Date(const Day::DayOfTheWeek dayOfTheWeek, const std::uint16_t d, const Month::MonthOfTheYear m,
		const std::uint16_t y)
		: day(d, dayOfTheWeek)
		, month(m)
		, year(y)
	{
		CheckDate();
	}

	Day Date::GetDay()
	{
		return day;
	}

	Month& Date::GetMonth()
	{
		return month;
	}

	Year& Date::GetYear()
	{
		return year;
	}

	std::string Date::ToString(const std::string_view& format) const
	{
		using TokenT = char;

		constexpr auto noMatchToken = type_trait::s_NullTerminator<char>;
		constexpr std::array<TokenT, 3> tokens{ 'd', 'm', 'y' };

		std::string output;
		output.reserve(format.size());

		size_t index = 0;
		for (auto letter = format.front();
			letter != type_trait::s_NullTerminator<char>;
			letter = format[++index])
		{
			TokenT match(noMatchToken);
			std::for_each(tokens.begin(), tokens.end(), [&](const TokenT pair)
			{
				if (pair == letter)
					match = pair;
			});

			if (match == type_trait::s_NullTerminator<char>)
			{
				output.push_back(letter);
				continue;
			}

			const auto first = format.find_first_of(match, index);
			auto last = format.find_first_not_of(match, first);

			if (last == std::string::npos)
				last = format.size();

			const auto count = last - first;
			std::string toAdd;
			toAdd.reserve(count);

			switch (kString::ToLower(match))
			{
			case 'd':
				toAdd = day.ToStringUsingTokenCount(count);
				break;

			case 'm':
				toAdd = month.ToStringUsingTokenCount(count);
				break;

			case 'y':
				toAdd = year.ToStringUsingTokenCount(count);
				break;
				
			default:
				throw kDebug::CalendarError("Bad format for date");
				break;
			}
			output.append(std::move(toAdd));
			index += count - 1;

			if (index >= format.size() - 1)
				break;
		}
		return output;
	}

	std::string Date::ToString(DateNumericalSeparator separator) const
	{
		std::string separatorStr;
		separatorStr += (separator);

		std::string str(
			kString::ToString("{0:2}{1}{2:2}{1}{3:4}"
				, GetDay()
				, separatorStr
				, GetMonth()
				, GetYear()));

		return str;
	}

	std::string Date::ToString(DateTextLength textLentgh) const
	{
		const auto dayStr = day.GetDayStr();
		const auto monthStr = month.GetMonthStr();
		const auto yearStr = year.GetYearStr();

		std::string str;

		switch (textLentgh)
		{
		case DateTextLength::FULL:
		{
			const auto d_o_t_wStr = day.GetDayOfTheWeekStr();
			str = kString::ToString("{0} {1} {2} {3}"
				, d_o_t_wStr
				, dayStr
				, monthStr
				, yearStr);
		}
		break;

		case DateTextLength::SHORT:
			str = kString::ToString("{0} {1} {2}"
				, monthStr
				, dayStr
				, yearStr
			);
			break;

		default:
			throw kDebug::CalendarError("Unknown date text format");
		}

		return str;
	}

	void Date::CheckDate() const
	{
		if (day.Verify())
			throw kDebug::CalendarError();

		if (month.Verify( day, year ))
			throw kDebug::InvalidMonthError();
	}

}

