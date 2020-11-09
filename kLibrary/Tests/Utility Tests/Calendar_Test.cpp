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
		VERIFY_MULTI(DayTest());
		VERIFY_MULTI(MonthTest());
		VERIFY_MULTI(YearTest());
		VERIFY_MULTI(MillisecondTest());
		VERIFY_MULTI(SecondTest());
		VERIFY_MULTI(MinuteTest());
		VERIFY_MULTI(HourTest());
		VERIFY_MULTI(ToStringTest());
		VERIFY_MULTI(CreateTimeTest());
		VERIFY_MULTI(GetTimeTextTest());
		VERIFY_MULTI(DayOfTheWeekTest());
		VERIFY_MULTI(GetTimeComponentTest());
		VERIFY_MULTI(GetDateInTextFormatTest());
		VERIFY_MULTI(GetDateInNumericalFormatTest());
		VERIFY_MULTI_END();
	}

	bool CalendarTester::DayTest()
	{
		{
			constexpr Day day(16, Day::WEDNESDAY);
			VERIFY_COMPILE_TIME(day.GetDay() == 16);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::WEDNESDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "16"
				&& day.ToString("dd") == "16");
			VERIFY(day.ToString("ddd") == "16th");
			VERIFY(day.ToString("dddd") == "Wed");
			VERIFY(day.ToString("ddddd") == "Wednesday");
		}
		
		{
			constexpr Day day(9, Day::MONDAY);
			VERIFY_COMPILE_TIME(day.GetDay() == 9);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::MONDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "9"
				&& day.ToString("dd") == "09");
			VERIFY(day.ToString("ddd") == "9th");
			VERIFY(day.ToString("dddd") == "Mon");
			VERIFY(day.ToString("ddddd") == "Monday");
		}
		
		{
			constexpr Day day(32, Day::THURSDAY);
			VERIFY_COMPILE_TIME(day.GetDay() == 32);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::THURSDAY);
			VERIFY(!day.Verify());
			VERIFY(day.ToString("d") == "32"
				&& day.ToString("dd") == "32");
			VERIFY(day.ToString("ddd") == "32nd");
			VERIFY(day.ToString("dddd") == "Thu");
			VERIFY(day.ToString("ddddd") == "Thursday");
		}

		return success;
	}

	bool CalendarTester::YearTest()
	{
		{
			constexpr Year year(2004);
			VERIFY_COMPILE_TIME(year.IsLeapYear());
			VERIFY_COMPILE_TIME(year.GetYear() == 2004);
			VERIFY_COMPILE_TIME(year.TotalDays() == 366);
			VERIFY(year.ToString("y") == "04"
				&& year.ToString("yy") == "04");
			VERIFY(year.ToString("yyy") == "2004");
			VERIFY(year.ToString("yyyy") == "2004");
		}

		{
			constexpr Year year(1862);
			VERIFY_COMPILE_TIME(!year.IsLeapYear());
			VERIFY_COMPILE_TIME(year.GetYear() == 1862);
			VERIFY_COMPILE_TIME(year.TotalDays() == 365);
			VERIFY(year.ToString("y") == "62"
				&& year.ToString("yy") == "62");
			VERIFY(year.ToString("yyy") == "1862");
			VERIFY(year.ToString("yyyy") == "1862");
		}

		return success;
	}

	bool CalendarTester::MonthTest()
	{
		{
			constexpr Month month(Month::MAR);
			constexpr Day day(7);
			VERIFY_COMPILE_TIME(month.GetMonthNumber() == 3);
			VERIFY_COMPILE_TIME(month.GetMonth() == Month::MAR);
			VERIFY(month.Verify(day));
			VERIFY(month.ToString("d") == "16"
				&& month.ToString("dd") == "16");
			VERIFY(month.ToString("ddd") == "16th");
			VERIFY(month.ToString("dddd") == "Wed");
			VERIFY(month.ToString("ddddd") == "Wednesday");
		}

		{
			constexpr Day day(9, Day::MONDAY);
			VERIFY_COMPILE_TIME(day.GetDay() == 9);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::MONDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "9"
				&& day.ToString("dd") == "09");
			VERIFY(day.ToString("ddd") == "9th");
			VERIFY(day.ToString("dddd") == "Mon");
			VERIFY(day.ToString("ddddd") == "Monday");
		}

		{
			constexpr Day day(32, Day::THURSDAY);
			VERIFY_COMPILE_TIME(day.GetDay() == 32);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::THURSDAY);
			VERIFY(!day.Verify());
			VERIFY(day.ToString("d") == "32"
				&& day.ToString("dd") == "32");
			VERIFY(day.ToString("ddd") == "32nd");
			VERIFY(day.ToString("dddd") == "Thu");
			VERIFY(day.ToString("ddddd") == "Thursday");
		}

		return success;
	}

	bool CalendarTester::MillisecondTest()
	{
	}

	bool CalendarTester::SecondTest()
	{
	}

	bool CalendarTester::MinuteTest()
	{
	}

	bool CalendarTester::HourTest()
	{
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
			const auto result = GetTimeText(CalendarSourceType::SYSTEM);
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
			const auto result = klib::kString::Convert<wchar_t>(GetTimeText(CalendarSourceType::SYSTEM));
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

	bool CalendarTester::ToStringTest()
	{
		{
			const Date date(Date::MONDAY, 2, Date::JAN, 1998);
			const auto result = date.ToString("dd/mm/yy");
			const auto expected = "02/01/98";
			VERIFY(expected == result);
		}

		{
			const Date date(Date::MONDAY, 12, Date::MAY, 2004);
			const auto result = date.ToString("dddd, ddd mmmmm yyyy");
			const auto expected = "Mon, 12th May 2004";
			VERIFY(expected == result);
		}

		{
			const Date date(Date::FRIDAY, 21);
			const auto result = date.ToString("ddddd, ddd");
			const auto expected = "Friday, 21st";
			VERIFY(expected == result);
		}

		{
			const Time t(12, 5, 20);
			const auto result = t.ToString("hh:mm:ss");
			const auto expected = "12:05:20";
			VERIFY(expected == result);
		}

		{
			const Time t(23, 10, 50);
			const auto result = t.ToString("hh:mm");
			const auto expected = "23:10";
			VERIFY(expected == result);
		}

		{
			const Time t(23, 10, 50, 34);
			const auto result = t.ToString("uuu");
			const auto expected = "034";
			VERIFY(expected == result);
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
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarSourceType::SYSTEM);
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
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarSourceType::SYSTEM);
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
			const auto result = GetDateInNumericalFormat(Date::SLASH, CalendarSourceType::SYSTEM);
			const auto expected = klib::kString::ToString("%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result
				= klib::kString::Convert<wchar_t>(GetDateInNumericalFormat(Date::DASH, CalendarSourceType::SYSTEM));
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
			const auto result = CreateTime(16, 18, 0, 567);
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

