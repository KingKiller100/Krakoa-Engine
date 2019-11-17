#pragma once

#include "../Tester.h"

namespace kTest::utility
{
	class CalendarTester : public Tester
	{
	public:
		CalendarTester();
		~CalendarTester() override;
		
	private:
		void Test() override;
	};
}

