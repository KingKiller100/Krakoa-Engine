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
		
		void InitializeMathsTests();
		void InitializeUtilityTests();
		void Add(Tester* test);
		void RunAll();
		void ClearAllTests();
	};
}