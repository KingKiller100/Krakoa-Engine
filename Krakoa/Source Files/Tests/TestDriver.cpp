#include "Precompile.hpp"
#include "TestDriver.hpp"

#include <Testing/Tester.hpp>
#include <Testing/TesterManager.hpp>

#include "SetUpTests.hpp"

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	auto& testMan = kTest::TesterManager::Get();

	void TestDriver::Initialize()
	{
		testMan.Initialize();
		testMan.InitializeMathsTests();
		testMan.InitializeUtilityTests();
		testMan.InitializeTemplateTests();
		
		SetUpTests();
	}

	void TestDriver::ShutDown()
	{
		testMan.Shutdown();
	}
	void TestDriver::AddTest(kTest::Tester* test)
	{
		testMan.Add(test);
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
