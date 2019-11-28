#include <Precompile.h>
#include <Testing/Utility Tests/Clock_Test.hpp>

#include <Utility/Clock/kClock.hpp>

namespace kTest::utility
{	
	ClockTester::ClockTester()
		: Tester("Clock Test")
	{
	}

	ClockTester::~ClockTester()
		= default;

	void ClockTester::Test()
	{
		util::kTime::Clock<float>::Start();
		util::kTime::Clock<double>::GetDeltaTime();
		util::kTime::Clock<long double>::GetSystemLifeTime();
	}
}
