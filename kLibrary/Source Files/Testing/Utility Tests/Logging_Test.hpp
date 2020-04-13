#pragma once
#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class LoggingTester : public Tester
	{
	public:
		LoggingTester();
		~LoggingTester() override;
		
	private:
		void Test() override;
		bool LogTest();

	private:
		std::string fullFilePathToDelete;
	};
}
#endif

