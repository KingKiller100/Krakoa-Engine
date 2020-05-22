#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Utility/Calendar/kCalendar.hpp"

#include <ctime>

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
		VERIFY_MULTI_END();

		const auto& localDateTimeTest = GetLocalDateAndTime();
		const auto& systemDateTimeTest = GetSystemDateAndTime();
		auto time = GetComponentOfTime(TimeComponent::hours);
		time = GetComponentOfTime(TimeComponent::mins);
		time = GetComponentOfTime(TimeComponent::secs);
		time = GetComponentOfTime(TimeComponent::millis);
		auto calendarTest = GetDateInNumericalFormat();
		calendarTest = GetDateInNumericalFormat(false);
		calendarTest = GetDateInTextFormat();
		calendarTest = GetTimeText();

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
}
#endif

