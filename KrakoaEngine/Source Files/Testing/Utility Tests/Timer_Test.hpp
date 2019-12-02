#pragma once

#include <Testing/Tester.hpp>

namespace kTest::utility
{
	class TimerTester : public Tester
	{
	public:
		TimerTester();
		~TimerTester() override;
		
	private:
		void Test() override;
	};
}
