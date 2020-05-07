#include "Precompile.hpp"
#include "../Core/TestDriver.hpp"

#include <Testing/Tester.hpp>
#include <Testing/TesterManager.hpp>

#ifdef KRAKOA_TEST
namespace tests
{
	auto& testMan = kTest::TesterManager::Get();

	void TestDriver::Initialize()
	{
		testMan.Initialize();
		testMan.InitializeMathsTests();
		testMan.InitializeUtilityTests();
	}

	void TestDriver::ShutDown()
	{
		testMan.Shutdown();
	}
	void TestDriver::AddTest(void* test)
	{
		testMan.Add(static_cast<kTest::Tester*>(test));
	}

	void TestDriver::RunAll()
	{
		testMan.RunAll();
		RunPerformanceTests();
	}

	void TestDriver::RunPerformanceTests()
	{
		testMan.RunPerformanceTests();
	}

	void TestDriver::ClearAll()
	{
		testMan.ClearAllTests();
	}
}
#endif
