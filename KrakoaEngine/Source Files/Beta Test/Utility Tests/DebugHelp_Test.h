#pragma once
#include "../Tester.h"

namespace kTest::utility
{
	class DebugHelpTester : public Tester
	{
	public:
		DebugHelpTester();
		~DebugHelpTester() override;
		
	private:
		bool Test() override;
	};
}


