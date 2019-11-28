#pragma once

#include <Testing/Tester.hpp>

namespace kTest::utility
{
	class ClockTester : public Tester
	{
	public:
		ClockTester();
		~ClockTester() override;
		
	private:
		void Test() override;
	};
}
