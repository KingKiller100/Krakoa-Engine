#include "pch.hpp"
#include "../Utility Tests/Timer_Test.hpp"

#include "../../Utility/Timer/kTimer.hpp"
#include "../../Utility/Format/kFormatToString.hpp"

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
		double sum(0);

		klib::kTime::HighAccuracyTimer testTime("Test Timer");

		const size_t size = 99999;
		int nums[size];
		for (auto i = 0; i < size; i++)
		{
			const auto dt = testTime.GetDeltaTime<klib::kTime::Micros>();
			nums[i] = i;
			sum += dt;
			VERIFY(nums[i] == i && dt != 0);
			s = klib::kFormat::ToString("Test Time %d : %dus (Microseconds)\n", i, dt);
			OutputDebugStringA(s.data());
		}

		std::cout << testTime.GetName() << " Total Test Time: " << testTime.GetLifeTime<klib::kTime::Micros>() / 1000000.0 << "s (Seconds)" << std::endl;
		std::cout << testTime.GetName() << " Average Execution Time: " << sum / size << "us (Microseconds)" << std::endl;
	}
}
#endif
