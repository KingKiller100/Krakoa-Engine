#pragma once

#include <Testing/Tester.hpp>

namespace kTest::utility
{
	class FormatToStringTester : public Tester
	{
	public:
		FormatToStringTester();
		~FormatToStringTester();

	private:
		void Test() override;

		void FormatToStringTest();
		void SingleObjectToStringTest();
	};
}