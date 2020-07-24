#pragma once

#include "../EngineMacros.hpp"

#ifdef KRAKOA_TEST
namespace kTest
{
	class Tester;
}

namespace krakoa::tests
{

	class KRAKOA_API TestDriver
	{
	public:
		static void Initialize();
		static void ShutDown();
		static void AddTest(kTest::Tester* test);
		static void RunAll();
		static void RunPerformanceTests();
		static void ClearAll();
	};
}
#endif
