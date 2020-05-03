#pragma once

#include "../HelperMacros.hpp"
#include <string>
#include <unordered_set>
#include <memory>

#ifdef TESTING_ENABLED
namespace kTest
{
	class Tester;
	
	class TesterManager
	{
	public:
		TesterManager();
		~TesterManager();

		void Initialize();
		void Shutdown();
		void InitializeMathsTests();
		void InitializeUtilityTests();
		void RunSpeedTests();
		void Add(Tester* test);
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		std::string path;
		std::unordered_set< std::unique_ptr<Tester> > testsUSet;

		bool success;
	};
}

#endif
