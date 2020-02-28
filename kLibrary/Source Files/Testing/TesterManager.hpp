#pragma once

#include <HelperMacros.hpp>

#include <Patterns/SimpleSingleton.hpp>

#ifdef TESTING_ENABLED
namespace kTest
{
	class Tester;	
	
	class TesterManager : public Pattern::SimpleSingleton<TesterManager>
	{
	public:
		TesterManager(Token);
		~TesterManager();

		static void Shutdown();

		static void Initialize();
		
		static void InitializeMathsTests();
		static void InitializeUtilityTests();
		static void Add(std::unique_ptr<Tester>&& test);
		static void RunAll();
		static void ClearAllTests();
	};
}

#endif
