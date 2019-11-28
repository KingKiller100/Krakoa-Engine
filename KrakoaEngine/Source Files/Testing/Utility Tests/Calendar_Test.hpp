#pragma once

#include <Testing/Tester.hpp>

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

