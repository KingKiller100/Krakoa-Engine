#pragma once

#include "../HelperMacros.hpp"

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
		void Add(Tester* test);
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		static TesterManager* pInstance;
	};
}

#endif
