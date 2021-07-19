#include "Precompile.hpp"
#include "TestDriver.hpp"

#include <iostream>

#ifdef KRAKOA_TEST
#include <Testing/TesterBase.hpp>
#include <Testing/TesterManager.hpp>
#include <Testing/SetUpTests.hpp>

#include "SetUpTests.hpp"

namespace krakoa::tests
{
	namespace 
	{
		kTest::TesterManager* testMan;
	}
	
	void TestDriver::Initialize()
	{
		testMan = new kTest::TesterManager{};
		testMan->Initialize();
		kTest::InitializeAllTests(testMan);
		SetUpTests();
	}

	void TestDriver::ShutDown()
	{
		testMan->Shutdown();
		delete testMan;
		testMan = nullptr;
	}
	void TestDriver::AddTest(kTest::TesterBase* test)
	{
		testMan->Add(test);
	}

	void TestDriver::RunAll()
	{
		testMan->RunAll(std::thread::hardware_concurrency());
		std::cin.get();
		RunPerformanceTests();
	}

	void TestDriver::RunPerformanceTests()
	{
		testMan->RunPerformanceTests();
	}

	void TestDriver::ClearAll()
	{
		testMan->ClearAllTests();
	}
}
#endif
