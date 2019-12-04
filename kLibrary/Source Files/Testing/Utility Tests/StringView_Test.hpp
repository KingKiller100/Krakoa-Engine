#pragma once
#include <Testing/Tester.hpp>

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

