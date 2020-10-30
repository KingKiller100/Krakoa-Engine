#include "pch.hpp"
#include "kDateTime.hpp"
#include "../String/kToString.hpp"
#include "../Debug Helper/Exceptions/CalenderExceptions.hpp"
#include <Windows.h>

namespace klib::kCalendar
{
	namespace
	{
		SYSTEMTIME& GetLocalDateAndTime() noexcept
		{
			static SYSTEMTIME kCalendar_Local_DateTime;
			GetLocalTime(&kCalendar_Local_DateTime);
			return kCalendar_Local_DateTime;
		}

		SYSTEMTIME& GetSystemDateAndTime() noexcept
		{
			static SYSTEMTIME kCalendar_System_DateTime;
			GetSystemTime(&kCalendar_System_DateTime);
			return kCalendar_System_DateTime;
		}

		const char* MonthToString(const size_t month)
		{
			static std::array<const char*, 12> kCalendar_MonthsArray = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			if (month < kCalendar_MonthsArray.size())
				return kCalendar_MonthsArray[month];

			throw kDebug::InvalidMonthError("Value entered does not index to a month of the year");
		}
		const char* DayOfTheWeekToString(const size_t day)
		{
			static std::array<const char*, 7> kCalendar_DaysOfTheWeek = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
			};

			if (day < kCalendar_DaysOfTheWeek.size())
				return kCalendar_DaysOfTheWeek[day];

			throw kDebug::InvalidDayError("Value entered does not index to a day of the week");
		}
	}

	// DATE ////////////////////////////////////////////////////////////

	Date::Date(CalendarInfoSource sourceInfo)
	{
		auto source = sourceInfo == CalendarInfoSource::LOCAL
			? GetLocalDateAndTime()
			: GetSystemDateAndTime();

		day = source.wDay;
		month = static_cast<MonthOfTheYear>(source.wMonth - 1);
		year = source.wYear;
		dayOfWeek = static_cast<DaysOfTheWeek>(source.wDayOfWeek);
	}

	Date::Date(const DaysOfTheWeek dayOfTheWeek, const DDMMYYYY_t d, const MonthOfTheYear m, const DDMMYYYY_t y)
		: day(d)
	, month(m)
	, year(y)
	, dayOfWeek(dayOfTheWeek)
	{
		CheckDate();
	}

	std::string Date::ToString(DateNumericalSeparator separator) const
	{
		std::string separatorStr;
		separatorStr += (separator);

		std::string str(
			kString::ToString("{0}{1}{2}{1}{3}"
			, GetDay()
			, separatorStr
			, GetMonthIndex()
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

	Date::DDMMYYYY_t Date::GetMonthIndex() const
	{
		return static_cast<DDMMYYYY_t>(month) + 1;
	}

	std::string Date::GetMonthStr() const
	{
		return MonthToString(month);
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

	Date::MonthOfTheYear Date::GetMonth() const
	{
		return month;
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
		if (day < 1)
			throw kDebug::CalendarError();
		
		if (month > 11)
			throw kDebug::InvalidMonthError();
		
		if (month == 1)
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
			if (month % 2 && day > 31)
			{
				throw kDebug::InvalidMonthError();
			}
			else if (day > 30)
			{
				throw kDebug::InvalidMonthError();
			}
		}
	}

	// TIME ////////////////////////////////////////////////////////////
	Time::Time(CalendarInfoSource sourceInfo)
	{
		const auto source = sourceInfo == CalendarInfoSource::LOCAL
			                    ? GetLocalDateAndTime()
			                    : GetSystemDateAndTime();
		
		hours = source.wHour;
		minutes = source.wMinute;
		seconds = source.wSecond;
		milliseconds = source.wMilliseconds;
	}

	Time::Time(const HHMMSSMS_t h, const HHMMSSMS_t m, const HHMMSSMS_t s, const HHMMSSMS_t ms)
		: hours(h) 
	, minutes(m)
	, seconds(s)
	, milliseconds(ms)
	{
		CheckTime();
	}
	
	void Time::CheckTime() const
	{
		if (hours > 23)
			kDebug::CalendarError("Invalid Hours");

		if (minutes > 59)
			kDebug::CalendarError("Invalid Minutes");

		if (seconds > 59)
			kDebug::CalendarError("Invalid Seconds");

		if (milliseconds > 999)
			kDebug::CalendarError("Invalid Milliseconds");
	}

	Time::HHMMSSMS_t Time::GetComponent(const TimeComponent timeComponent) const
	{
		switch (timeComponent) {
		case TimeComponent::HOURS:		return hours;
		case TimeComponent::MINS:		return minutes;
		case TimeComponent::SECS:		return seconds;
		case TimeComponent::MILLIS:		return milliseconds;
		default: throw kDebug::CalendarError("Unknown time component");
		}
	}

	std::string Time::ToString(const TimeComponent accuracy) const
	{		
		std::string str;
		HHMMSSMS_t times[] = { hours, minutes, seconds };
		
		for (auto i = CAST(HHMMSSMS_t, TimeComponent::HOURS);
			i <= CAST(HHMMSSMS_t, accuracy);
			++i)
		{
			if (!str.empty())
				str.push_back(':');
			str.append(i >= CAST(HHMMSSMS_t, TimeComponent::MILLIS)
				? kString::ToString("{0:3}", milliseconds)
				: kString::ToString("{0:2}", times[i]));
		}
		
		return str;
	}

	Time::HHMMSSMS_t Time::GetHours() const
	{
		return hours;
	}

	Time::HHMMSSMS_t Time::GetMinutes() const
	{
		return minutes;
	}

	Time::HHMMSSMS_t Time::GetSeconds() const
	{
		return seconds;
	}

	Time::HHMMSSMS_t Time::GetMilliseconds() const
	{
		return milliseconds;
	}
}
