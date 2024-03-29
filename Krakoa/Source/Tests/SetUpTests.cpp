﻿#include "Precompile.hpp"
#include "SetUpTests.hpp"

#include "TestDriver.hpp"

#include "../../Tests/ManagerBase_Test.hpp"
#include "../../Tests/MemoryTracker_Test.hpp"

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	void SetUpTests() noexcept
	{
		//TestDriver::AddTest(new MemPoolerTester()); // Crashing system
		TestDriver::AddTest(new ManagerBaseTester());
		TestDriver::AddTest(new MemoryTrackerTester());
	}
}
#endif