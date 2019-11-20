#include "Precompile.h"
#include "Calendar_Test.h"

#include "../../Utility/Calendar/kCalendar.h"

namespace kTest::utility
{
	using namespace util;
	
	CalendarTester::CalendarTester()
		: Tester("Calendar Test")
	{
	}

	CalendarTester::~CalendarTester()
		= default;

	bool CalendarTester::Test()
	{
		const auto& localDateTimeTest = kCalendar::GetLocalDateAndTime();
		const auto& systemDateTimeTest = kCalendar::GetSystemDateAndTime();
		auto time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::hour);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::min);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::sec);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::millisec);
		auto calendarTest = kCalendar::GetDateInNumericalFormat();
		calendarTest = kCalendar::GetDateInNumericalFormat(false);
		calendarTest = kCalendar::GetDateInTextFormat();
		calendarTest = kCalendar::GetTimeText();
		calendarTest = kCalendar::GetDayOfTheWeek(3);
		calendarTest = kCalendar::GetMonth(3);	

		return true;
	}
}

