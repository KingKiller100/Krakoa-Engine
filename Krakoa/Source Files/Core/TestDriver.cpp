#include "Precompile.hpp"
#include "../Core/TestDriver.hpp"

#include <Testing/Tester.hpp>
#include <Testing/TesterManager.hpp>

#ifdef KRAKOA_TEST
namespace tests
{
	auto& testMan = kTest::TesterManager::Get();

	TestDriver::TestDriver()
	{	}

	void TestDriver::Initialize()
	{
		testMan.Initialize();
		testMan.InitializeMathsTests();
		testMan.InitializeUtilityTests();
		testMan.InitializeSpeedTests();
	}

	void TestDriver::ShutDown()
	{
		testMan.Shutdown();
	}
	void TestDriver::AddTest(void* test)
	{
		testMan.Add(static_cast<kTest::Tester*>(test));
	}
	void TestDriver::RunTests()
	{
		testMan.RunAll();
	}
	void TestDriver::ClearAll()
	{
		testMan.ClearAllTests();
	}
}
#endif
