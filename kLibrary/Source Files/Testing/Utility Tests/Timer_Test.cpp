#include "pch.hpp"
#include "../Utility Tests/Timer_Test.hpp"

#include "../../Maths/kAlgorithms.hpp"
#include "../../Utility/Timer/kTimer.hpp"
#include "../../Utility/Format/kFormatToString.hpp"

#include <iostream>
#include <thread>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	TimerTester::TimerTester()
		: Tester("Timer Test")
	{	}

	TimerTester::~TimerTester()
		= default;

	void TimerTester::Test()
	{
		std::string s;

		const auto maxCount = 99999;
		int nums[maxCount];

		const auto processorCount = std::thread::hardware_concurrency();
		const size_t size = 5000 * processorCount;

		const auto loops = kmaths::Min(size, maxCount);

		klib::kTime::HighAccuracyTimer testTime("Test Timer");

		for (auto i = 0; i < loops; i++)
		{
			const auto dt = testTime.GetDeltaTime<klib::kTime::kUnits::Micros>();
			nums[i] = i;
			VERIFY(nums[i] == i && dt != 0);
			s = klib::kFormat::ToString("Test Time %d : %fus (Microseconds)\n", i, dt);
		}

		const auto lifetime = testTime.GetLifeTime<klib::kTime::kUnits::Micros>();
		const auto lifetimeInSeconds = lifetime / 1000000;
		std::cout << testTime.GetName() << " Total Test Time: " << lifetimeInSeconds << "s (Seconds)" << std::endl;
		std::cout << testTime.GetName() << " Average Execution Time: " << lifetime / loops << "us (Microseconds)" << std::endl;

		//std::cin.get();
	}
}
#endif
