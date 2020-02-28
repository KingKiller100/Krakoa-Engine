#pragma once

#ifdef KRAKOA_TEST
namespace tests
{
	class TestDriver
	{
	public:
		TestDriver() 
			= default;
		void Initialize();
		void ShutDown();
		void AddTest(void* test);
		void RunTests();
		void ClearAll();
	};
}
#endif
