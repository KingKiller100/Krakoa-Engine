#include "Precompile.hpp"
#include "TestDriver.hpp"

#ifdef KRAKOA_TEST
#include <Testing/TesterBase.hpp>
#include <Testing/TesterManager.hpp>

#include "SetUpTests.hpp"

namespace krakoa::tests
{
	auto& testMan = kTest::TesterManager::Get();

	void TestDriver::Initialize()
	{
		testMan.Initialize();
		testMan.InitializeMaths();
		testMan.InitializeUtility();
		testMan.InitializeTemplates();
		
		SetUpTests();
	}

	void TestDriver::ShutDown()
	{
		testMan.Shutdown();
	}
	void TestDriver::AddTest(kTest::TesterBase* test)
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
