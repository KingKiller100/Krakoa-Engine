#include <Precompile.h>
#include <Testing/Utility Tests/Timer_Test.hpp>

#include <Utility/Clock/kTimer.hpp>
#include <Utility/Format/kFormatToString.hpp>

#include <vector>

namespace kTest::utility
{	
	TimerTester::TimerTester()
		: Tester("Clock Test")
	{
	}

	TimerTester::~TimerTester()
		= default;

	void TimerTester::Test()
	{
		util::kTime::BriefTimer testTime("Clock Test Timer");

		std::vector<int> nums;
		for (auto i = 0; i < 100; i++)
		{
			const auto name = util::kFormat::FormatToString("Timer %d", i);
			const auto emplace_backTimer = util::kTime::HighAccuracyTimer(name.c_str());
			nums.emplace_back(i);
			std::cout << emplace_backTimer.GetName() <<  ": " << emplace_backTimer.GetLifeTime<std::chrono::microseconds>() << "us\n" ;
		}

		std::cout << testTime.GetName() << " Time: " << testTime.GetLifeTime() << "ms\n";
	}
}
