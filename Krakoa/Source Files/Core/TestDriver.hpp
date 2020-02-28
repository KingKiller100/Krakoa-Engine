#pragma once

#ifdef KRAKOA_TEST
namespace tests
{
	class TestDriver
	{
	public:
		TestDriver();
		void Initialize();
		void ShutDown();
		void AddTest(void* test);
		void RunTests();
		void ClearAll();
	};
}
#endif
