#pragma once

#include "../Core/EngineCore.h"

#include "../Patterns/SimpleSingleton.h"

namespace kTest
{
	class Tester;	
	
	class KRAKOA_API TesterManager : public Pattern::SimpleSingleton<TesterManager>
	{
	public:
		TesterManager(Token);
		~TesterManager();

		static void Shutdown();
		
		static void InitializeMathsTests();
		static void InitializeUtilityTests();
		static void Add(Tester* test);
		static void RunAll();
		static void ClearAllTests();
	};
}