#pragma once

#include "../Core/EngineCore.hpp"

#ifdef KRAKOA_TEST
namespace tests
{
	class KRAKOA_API TestDriver
	{
	public:
		static void Initialize();
		static void ShutDown();
		static void AddTest(void* test);
		static void RunAll();
		static void RunPerformanceTests();
		static void ClearAll();
	};
}
#endif
