#include <pch.hpp>

#ifdef _WIN64

#include "../../Date/kDate.hpp"

#include <algorithm>


#include "kCalenderInfoSourceWindows.hpp"

#include "../../../../Type Traits/StringTraits.hpp"

#include "../../../Debug Helper/Exceptions/CalenderExceptions.hpp"


#include "../../../String/kToString.hpp"

namespace klib::kCalendar
{
	using namespace secret::helper::windows;

	// DATE ////////////////////////////////////////////////////////////
	Date::Date(const _SYSTEMTIME& source)
		: day(source.wDay)
		, monthIndex(static_cast<MonthOfTheYear>(source.wMonth - 1))
		, year(source.wYear)
		, dayOfWeek(static_cast<DaysOfTheWeek>(source.wDayOfWeek))
	{}


	Date::Date(CalendarInfoSourceType sourceType)
		: Date(secret::helper::windows::GetDateAndTime(sourceType))
	{}

	Date::Date(const DaysOfTheWeek dayOfTheWeek, const DDMMYYYY_t d, const MonthOfTheYear m, const DDMMYYYY_t y)
		: day(d)
		, monthIndex(m)
		, year(y)
		, dayOfWeek(dayOfTheWeek)
	{
		CheckDate();
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
			{
				if (count >= 5)
					toAdd = kString::ToString("{0}", GetDayOfWeekStr());
				else if (count == 4)
					toAdd = kString::ToString("{0}", GetDayOfWeekStr().substr(0, 3));
				else if (count == 3)
					toAdd = kString::ToString("{0}", GetDayStr());
				else if (count == 2)
					toAdd = kString::ToString("{0:2}", GetDay());
				else
					toAdd = kString::ToString("{0}", GetDay());
			}
			break;
			case 'm':
			{
				if (count >= 4)
					toAdd = kString::ToString("{0}", GetMonthStr());
				else if (count == 3)
					toAdd = kString::ToString("{0}", GetMonthStr().substr(0, 3));
				else if (count == 2)
					toAdd = kString::ToString("{0:2}", GetMonth());
				else
					toAdd = kString::ToString("{0}", GetMonth());
			}
			break;
			case 'y':
			{
				if (count <= 2)
					toAdd = GetYearStr().substr(2);
				else
					toAdd = GetYearStr();
			}
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

	std::string Date::ToString(DateTextLength format) const
	{
		const auto dayStr = GetDayStr();
		const auto monthStr = GetMonthStr();
		const auto yearStr = GetYearStr();

		std::string str;

		switch (format)
		{
		case DateTextLength::FULL:
		{
			const auto d_o_t_wStr = GetDayOfWeekStr();
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

	Date::DDMMYYYY_t Date::GetDay() const
	{
		return day;
	}

	std::string Date::GetDayStr() const
	{
		const auto dateSuffix = [&]()
		{
			return (day == 1 || day == 21 || day == 31) ? "st"
				: (day == 2 || day == 22) ? "nd"
				: day == 3 ? "rd"
				: "th";
		};
		const auto str = kString::ToString("{0}{1}"
			, day
			, dateSuffix());
		return str;
	}

	Date::DDMMYYYY_t Date::GetMonth() const
	{
		return static_cast<DDMMYYYY_t>(monthIndex) + 1;
	}

	std::string Date::GetMonthStr() const
	{
		return MonthToString(monthIndex);
	}

	std::string Date::GetYearStr() const
	{
		const auto str = kString::ToString("{0}", year);
		return str;
	}

	Date::DaysOfTheWeek Date::GetDayOfWeek() const
	{
		return dayOfWeek;
	}

	Date::DDMMYYYY_t Date::GetYear() const
	{
		return year;
	}

	std::string Date::GetDayOfWeekStr() const
	{
		const auto* const str = DayOfTheWeekToString(dayOfWeek);
		return str;
	}

	void Date::CheckDate() const
	{
		if (day == 0)
			throw kDebug::CalendarError();

		if (monthIndex > 11)
			throw kDebug::InvalidMonthError();

		if (monthIndex == 1)
		{
			if (year % 4 && day > 29)
			{
				throw kDebug::InvalidMonthError();
			}
			else if (day > 28)
			{
				throw kDebug::InvalidMonthError();
			}
		}
		else
		{
			if (monthIndex % 2 && day > 31)
			{
				throw kDebug::InvalidMonthError();
			}
			if (day > 30)
			{
				throw kDebug::InvalidMonthError();
			}
		}
	}

}
#endif
