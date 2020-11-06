#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class CalendarTester : public Tester
	{
	public:
		CalendarTester();
		~CalendarTester() override;
		
	private:
		void Test() override;

		bool MonthTest();
		bool GetTimeTextTest();
		bool DayOfTheWeekTest();

		bool ToStringTest();
		
		bool GetTimeComponentTest();
		bool GetDateInTextFormatTest();
		bool GetDateInNumericalFormatTest();

		bool CreateTimeTest();
	};
}
#endif

