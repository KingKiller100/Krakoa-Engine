#include "Precompile.h"
#include "Clock_Test.h"

#include "../../Utility/Clock/kClock.h"

namespace kTest::utility
{
	using namespace util;
	
	ClockTester::ClockTester()
		: Tester("Clock Test")
	{
	}

	ClockTester::~ClockTester()
		= default;

	bool ClockTester::Test()
	{
		kTime::Clock<float>::Start();
		kTime::Clock<double>::GetDeltaTime();
		kTime::Clock<long double>::GetSystemLifeTime();

		return true;
	}
}
