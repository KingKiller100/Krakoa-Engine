#pragma once
#include "../Tester.h"


namespace kTest::utility
{
	class StringViewTester : public Tester
	{
	public:
		StringViewTester();
		~StringViewTester() override;

	private:
		void Test() override;				
	};

}
