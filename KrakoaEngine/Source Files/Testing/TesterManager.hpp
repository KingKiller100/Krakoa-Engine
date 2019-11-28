#pragma once

#include <Core/EngineCore.hpp>

#include <Patterns/SimpleSingleton.hpp>

namespace kTest
{
	class Tester;	
	
	class KRAKOA_API TesterManager : public Pattern::SimpleSingleton<TesterManager>
	{
	public:
		TesterManager(Token);
		~TesterManager();

		static void Shutdown();

		static void Initialize();
		
		static void InitializeMathsTests();
		static void InitializeUtilityTests();
		static void Add(Tester* test);
		static void RunAll();
		static void ClearAllTests();
		
	};
}