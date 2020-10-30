#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Source Files/Utility/Calendar/kCalendar.hpp"

#include <Windows.h>

#ifdef TESTING_ENABLED
namespace kTest::utility
{

	CalendarTester::CalendarTester()
		: Tester("Calendar Test")
	{}

	CalendarTester::~CalendarTester()
		= default;

	using namespace klib::kCalendar;

	template<typename T, typename Arg, typename  ...Args>
	std::basic_string<T> MakeString(const T* format, Arg arg, Args ...args)
	{
		T* buffer = nullptr;

		if _CONSTEXPR_IF(std::is_same_v<T, char>)
		{
			const auto length = 1 + std::snprintf(nullptr, 0, format, arg, args...);
			if (length <= 0) throw std::exception();
			buffer = new T[length]{};
			sprintf_s(buffer, length, format, arg, args...);
		}
		else if _CONSTEXPR_IF(std::is_same_v<T, wchar_t>)
		{
			const auto length = 1 + _snwprintf_l(nullptr, 0, format, nullptr, arg, args...);
			if (length <= 0) throw std::exception();
			buffer = new T[length]{};
			swprintf_s(buffer, length, format, arg, args...);
		}

		std::basic_string<T> str = buffer;
		delete[] buffer;
		return str;
	}

	void CalendarTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(MonthTest());
		VERIFY_MULTI(CreateTimeTest());
		VERIFY_MULTI(GetTimeTextTest());
		VERIFY_MULTI(DayOfTheWeekTest());
		VERIFY_MULTI(GetTimeComponentTest());
		VERIFY_MULTI(GetDateInTextFormatTest());
		VERIFY_MULTI(GetDateInNumericalFormatTest());
		VERIFY_MULTI_END();
	}

	bool CalendarTester::MonthTest()
	{
		{
			const Date date(Date::DaysOfTheWeek::FRIDAY, 1, Date::MAR, 2020);
			const auto result = date.GetMonthStr();
			VERIFY(result.compare("March") == 0);
		}

		{
			const Date date(Date::DaysOfTheWeek::FRIDAY, 1, Date::JUN, 2020);
			const auto result = date.GetMonthStr();
			VERIFY(result.compare("June") == 0);
		}

		{
			const Date date(Date::DaysOfTheWeek::FRIDAY, 1, Date::DEC, 2020);
			const auto result = date.GetMonthStr();
			VERIFY(result.compare("December") == 0);
		}

		return success;
	}

	bool CalendarTester::GetTimeTextTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetTimeText();
			const auto expected = MakeString("%02d:%02d:%02d:%03d", 
				localTime.wHour, 
				localTime.wMinute, 
				localTime.wSecond, 
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = klib::kString::Convert<wchar_t>(GetTimeText());
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d", 
				localTime.wHour, 
				localTime.wMinute, 
				localTime.wSecond, 
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetTimeText(CalendarInfoSource::SYSTEM);
			const auto expected = MakeString("%02d:%02d:%02d:%03d", 
				systemTime.wHour, 
				systemTime.wMinute, 
				systemTime.wSecond, 
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = klib::kString::Convert<wchar_t>(GetTimeText(CalendarInfoSource::SYSTEM));
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d", 
				systemTime.wHour, 
				systemTime.wMinute, 
				systemTime.wSecond, 
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}

		
		return success;
	}

	bool CalendarTester::DayOfTheWeekTest()
	{
		{
			Date date = Date(Date::DaysOfTheWeek::TUESDAY);
			const auto result = date.GetDayOfWeekStr();
			VERIFY(result == "Tuesday");
		}

		{
			Date date = Date(Date::DaysOfTheWeek::FRIDAY);
			const auto result = date.GetDayOfWeekStr();
			VERIFY(result == "Friday");
		}

		{
			Date date = Date(Date::DaysOfTheWeek::SUNDAY);
			const auto result = date.GetDayOfWeekStr();
			VERIFY(result == "Sunday");
		}

		return success;
	}

	bool CalendarTester::GetTimeComponentTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto hour = GetComponentOfTime(Time::HOURS);
			VERIFY(hour == localTime.wHour);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto mins = GetComponentOfTime(Time::MINS);
			VERIFY(mins == localTime.wMinute);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto secs = GetComponentOfTime(Time::SECS);
			VERIFY(secs == localTime.wSecond);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto millis = GetComponentOfTime(Time::MILLIS);
			VERIFY(millis == localTime.wMilliseconds);
		}


		return success;
	}

	auto DateTextFunc(const SYSTEMTIME dateTime, Date::DateTextLength format = Date::DateTextLength::FULL) -> decltype(auto)
	{
		constexpr std::array<const char*, 12> months = {
			"January", "February", "March",
			"April", "May", "June",
			"July", "August", "September",
			"October", "November", "December"
		};

		constexpr  std::array<const char*, 7> days = {
			"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
		};

		const std::string day = days[dateTime.wDayOfWeek];
		std::string month = months[dateTime.wMonth - 1];

		const auto dateSuffix = [&]()
			{
				return (dateTime.wDay == 1 || dateTime.wDay == 21 || dateTime.wDay == 31) ? "st"
					: (dateTime.wDay == 2 || dateTime.wDay == 22) ? "nd"
					: dateTime.wDay == 3 ? "rd"
					: "th";
			};

		std::string dateStr;
		if (format == Date::FULL)
		{
			dateStr = MakeString<char>("%s %d%s %s %04d",
				day.data(),
				dateTime.wDay,
				dateSuffix(),
				month.data(),
				dateTime.wYear);
		}
		else
		{
			
			dateStr = MakeString<char>("%s %d%s %d",
				month.data(),
				dateTime.wDay,
				dateSuffix(),
				dateTime.wYear);
		}

		return dateStr;
	};



	bool CalendarTester::GetDateInTextFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::FULL);
			const auto expected = DateTextFunc(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::SHORT);
			const auto expected = DateTextFunc(localTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarInfoSource::SYSTEM);
			const auto expected = DateTextFunc(systemTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::DateTextLength::FULL);
			const auto expected = DateTextFunc(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::DateTextLength::SHORT);
			const auto expected = DateTextFunc(localTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarInfoSource::SYSTEM);
			const auto expected = DateTextFunc(systemTime, Date::SHORT);
			VERIFY(result == expected);
		}


		return success;
	}

	bool CalendarTester::GetDateInNumericalFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = klib::kString::Convert<wchar_t>(GetDateInNumericalFormat(Date::SLASH));
			const auto expected = klib::kString::ToString(L"%02d/%02d/%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInNumericalFormat(Date::DASH);
			const auto expected = klib::kString::ToString("%02d-%02d-%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInNumericalFormat(Date::SLASH, CalendarInfoSource::SYSTEM);
			const auto expected = klib::kString::ToString("%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result
				= klib::kString::Convert<wchar_t>(GetDateInNumericalFormat(Date::DASH, CalendarInfoSource::SYSTEM));
			const auto expected = klib::kString::ToString(L"%02d-%02d-%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}


		return success;
	}

	bool CalendarTester::CreateTimeTest()
	{
		{
			const auto result = CreateTime(12, 30, 25);
			constexpr const auto* const expected = "12:30:25";
			VERIFY(result == expected);
		}
		
		{
			const auto result= CreateTime(16, 18, 0, 567);
			constexpr const auto* const expected = "16:18:00:567";
			VERIFY(result == expected);
		}
		
		{
			const auto result = CreateTime(6, 2, 8, 24);
			constexpr const auto* const expected = "06:02:08:024";
			VERIFY(result == expected);
		}
		
		{
			const auto result = CreateTime(6, 20);
			constexpr const auto* const expected = "06:20";
			VERIFY(result == expected);
		}
		
		{
			const auto result = CreateTime(24, 60, 60);

			constexpr const auto* const bad = "24:60:60";
			VERIFY(result != bad);
			constexpr const auto* const expected = "23:59:59";
			VERIFY(result == expected);
		}

		
		return success;
	}
}
#endif

