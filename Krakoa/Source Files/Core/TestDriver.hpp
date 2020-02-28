#pragma once

#ifdef KRAKOA_TEST
namespace tests
{
	class TestDriver
	{
	public:
		static void Initialize();
		static void ShutDown();
		static void AddTest(void* test);
		static void RunTests();
		static void ClearAll();
	};
}
#endif
