#pragma once
#include <Testing/Tester.hpp>

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
	};
}
#endif

