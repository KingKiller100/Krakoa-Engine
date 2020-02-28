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

		void Initialize();
		void Shutdown();
		void InitializeMathsTests();
		void InitializeUtilityTests();
		void Add(std::unique_ptr<Tester>&& test);
		void Add(std::unique_ptr<Tester>& test);
		void RunAll();
		void ClearAllTests();
	};
}

#endif
