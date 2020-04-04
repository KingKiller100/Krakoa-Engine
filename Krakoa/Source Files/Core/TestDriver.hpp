#pragma once

#include "../Core/EngineCore.hpp"

#ifdef KRAKOA_TEST
namespace tests
{
	class KRAKOA_API TestDriver
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
