#include <pch.hpp>
#include <Testing/Utility Tests/Timer_Test.hpp>

#include <Utility/Timer/kTimer.hpp>

namespace kTest::utility
{	
	TimerTester::TimerTester()
		: Tester("Clock Test")
	{	}

	TimerTester::~TimerTester()
		= default;

	void TimerTester::Test()
	{
		using namespace klib::kTime;

		HighAccuracyTimer testTime("Clock Test Timer");

		std::vector<int> nums;
		for (auto i = 0; i < 1000; i++)
		{
			nums.emplace_back(i);
			const auto dt = testTime.GetDeltaTime<Micros>();
			VERIFY(nums[i] == i && dt != 0);
			std::cout << "Emplace_back Test " << i << ": " << dt << "us (Microseconds)\n";
		}

		std::cout << testTime.GetName() << " Time: " << testTime.GetLifeTime<Millis>() << "ms (Milliseconds)\n";
	}
}
