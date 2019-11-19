#pragma once

#include "../Tester.h"

namespace kTest::utility
{
	class LoggingTester : public Tester
	{
	public:
		LoggingTester();
		~LoggingTester() override;
		
	private:
		bool Test() override;
	};
}

