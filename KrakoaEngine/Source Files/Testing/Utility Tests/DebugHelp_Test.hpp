#pragma once
#include <Testing/Tester.hpp>

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


