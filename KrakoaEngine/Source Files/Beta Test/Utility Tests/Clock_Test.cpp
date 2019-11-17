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

	void ClockTester::Test()
	{
		kTime::Clock<float>::Start();
		kTime::Clock<double>::GetDeltaTime();
		util::kTime::Clock<long double>::GetSystemLifeTime();		
	}
}
