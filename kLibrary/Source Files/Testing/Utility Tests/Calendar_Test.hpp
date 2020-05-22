#pragma once
#include "../Tester.hpp"

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
		bool DayOfTheWeekTest();
	};
}
#endif

