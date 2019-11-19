#pragma once

#include "../Tester.h"

namespace kTest::utility
{
	class ClockTester : public Tester
	{
	public:
		ClockTester();
		~ClockTester() override;
		
	private:
		bool Test() override;
	};
}
