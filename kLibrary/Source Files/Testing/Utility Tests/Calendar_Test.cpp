#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Utility/Calendar/kCalendar.hpp"

#include <chrono>
#include <ctime>

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

	void CalendarTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(MonthTest);
		VERIFY_MULTI(DayOfTheWeekTest);
		VERIFY_MULTI(GetTimeComponentTest);
		VERIFY_MULTI(GetDateInTextFormatTest);
		VERIFY_MULTI(GetDateInNumericalFormatTest);
		VERIFY_MULTI_END();

		const auto& localDateTimeTest = GetLocalDateAndTime();
		const auto calendarTest = GetTimeText();

		const auto timeStamp = __TIMESTAMP__;
		const auto date = __TIME__;
	}

	bool CalendarTester::MonthTest()
	{
		{
			constexpr auto result = GetMonth(2);
			VERIFY_COMPILE_TIME(result.compare("March") == 0);
		}

		{
			constexpr auto result = GetMonth(5);
			VERIFY_COMPILE_TIME(result.compare("June") == 0);
		}

		{
			constexpr auto result = GetMonth(11);
			VERIFY_COMPILE_TIME(result.compare("December") == 0);
		}

		{
			try
			{
				const auto result = GetMonth(12);
			}
			catch (const klib::kDebug::CalendarError & e)
			{
				constexpr std::string_view error = "Value entered does not index to a month of the year";
				VERIFY(error.compare(e.what()) == 0);
			}
		}

		return success;
	}

	bool CalendarTester::DayOfTheWeekTest()
	{
		{
			constexpr auto result = GetDayOfTheWeek(2);
			VERIFY_COMPILE_TIME(result.compare("Tuesday") == 0);
		}

		{
			constexpr auto result = GetDayOfTheWeek(5);
			VERIFY_COMPILE_TIME(result.compare("Friday") == 0);
		}

		{
			constexpr auto result = GetDayOfTheWeek(0);
			VERIFY_COMPILE_TIME(result.compare("Sunday") == 0);
		}

		{
			try
			{
				const auto result = GetDayOfTheWeek(7);
			}
			catch (const klib::kDebug::CalendarError & e)
			{
				constexpr std::string_view error = "Value entered does not index to a day of the week";
				VERIFY(error.compare(e.what()) == 0);
			}
		}

		return success;
	}

	bool CalendarTester::GetTimeComponentTest()
	{
		SYSTEMTIME localTime;
		::GetLocalTime(&localTime);

		const auto hour = GetComponentOfTime(TimeComponent::HOURS);
		const auto mins = GetComponentOfTime(TimeComponent::MINS);
		const auto secs = GetComponentOfTime(TimeComponent::SECS);
		const auto millis = GetComponentOfTime(TimeComponent::MILLIS);

		VERIFY(hour == localTime.wHour);
		VERIFY(mins == localTime.wMinute);
		VERIFY(secs == localTime.wSecond);
		VERIFY(millis == localTime.wMilliseconds);

		return success;
	}

		template<typename T >
		constexpr auto DateTextFunc(const WORD dayIndex, const WORD monthIndex, const SYSTEMTIME dateTime) -> decltype(auto)
		{
			constexpr std::array<const char*, 12> months = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			constexpr std::array<const wchar_t*, 12> wMonths = {
				L"January", L"February", L"March",
				L"April", L"May", L"June",
				L"July", L"August", L"September",
				L"October", L"November", L"December"
			};

			constexpr  std::array<const char*, 7> days = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
			};

			constexpr  std::array<const wchar_t*, 7> wDays = {
				L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
			L"Thursday", L"Friday", L"Saturday"
			};

			std::basic_string<T> day;
			std::basic_string<T> month;
			std::basic_string<T> dateStr;

			if _CONSTEXPR_IF(std::is_same_v<T, char>)
			{
				day = days[dayIndex];
				month = months[monthIndex];

				dateStr = klib::kFormat::ToString("%s %d %s %04d",
					day.data(),
					dateTime.wDay,
					GetMonth(dateTime.wMonth - 1).data(),
					dateTime.wYear);
			}
			else if _CONSTEXPR_IF(std::is_same_v<T, wchar_t>)
			{
				day = wDays[dayIndex];
				month = wMonths[monthIndex];

				dateStr = klib::kFormat::ToString(L"%s %d %s %04d",
					day.data(),
					dateTime.wDay,
					GetMonth(dateTime.wMonth - 1).data(),
					dateTime.wYear);
			}


			return dateStr;
		};



	bool CalendarTester::GetDateInTextFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat();
			const auto expected = DateTextFunc<char>(localTime.wDayOfWeek, localTime.wMonth, localTime);
			VERIFY(result == expected);
		}

		return success;
	}

	bool CalendarTester::GetDateInNumericalFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = wGetDateInNumericalFormat();
			const auto expected = klib::kFormat::ToString(L"%02d/%02d/%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInNumericalFormat(DateSeparator::DASH);
			const auto expected = klib::kFormat::ToString("%02d-%02d-%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInNumericalFormat(DateSeparator::SLASH, GetSystemDateAndTime());
			const auto expected = klib::kFormat::ToString("%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = wGetDateInNumericalFormat(DateSeparator::DASH, GetSystemDateAndTime());
			const auto expected = klib::kFormat::ToString(L"%02d-%02d-%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}


		return success;
	}
}
#endif

