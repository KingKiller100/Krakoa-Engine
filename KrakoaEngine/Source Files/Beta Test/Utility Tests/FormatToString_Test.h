#pragma once

#include "../Tester.h"

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