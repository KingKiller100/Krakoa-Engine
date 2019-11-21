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
		void Test() override;
	};
}


