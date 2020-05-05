#pragma once

#include "../HelperMacros.hpp"
#include <string>
#include <set>
#include <memory>

#ifdef TESTING_ENABLED
namespace kTest
{
	class Tester;
	
	class TesterManager
	{
		struct Token {};
	public:
		TesterManager(Token&);
		~TesterManager();

		void Initialize();
		void Shutdown();
		void InitializeMathsTests();
		void InitializeUtilityTests();
		void RunPerformanceTests();
		void Add(Tester* test);
		void Run(Tester* test);
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		std::string path;
		std::set< std::unique_ptr<Tester> > testsUSet;

		bool success;
	};
}

#endif
