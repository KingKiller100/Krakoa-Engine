#pragma once

#include "../Tester.hpp"

#include <string>
#include <unordered_set>


#ifdef TESTING_ENABLED
namespace kTest::speed
{
	class SpeedTestBase;

	class SpeedTestManager : public Tester
	{
	public:
		SpeedTestManager();
		~SpeedTestManager();

		void Add(SpeedTestBase* test);
		void ClearAll();
		
		void CollectResult(const std::string_view& result);
		
		static SpeedTestManager& Get();
	
	private:
		void Initialize();
		void ShutDown();
		void Run();

		void InitializeMaths();

		void Test() override;
		void OutputResult(const std::string& name);

	private:
		std::unordered_set<SpeedTestBase*> tests;
		std::string results;
		std::string directory;
	};
}
#endif