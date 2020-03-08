#include <Precompile.hpp>
#include <Core/TestDriver.hpp>

#ifdef KRAKOA_TEST
namespace tests
{
	auto& testMan = kTest::TesterManager::Reference();

	TestDriver::TestDriver()
	{	}

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
		auto tPtr = (kTest::Tester*)test;
		testMan.Add(std::unique_ptr<kTest::Tester>(tPtr));
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
