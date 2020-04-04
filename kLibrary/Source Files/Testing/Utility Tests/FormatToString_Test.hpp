#pragma once
#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FormatToStringTester : public Tester
	{
	public:
		FormatToStringTester();
		~FormatToStringTester();

	private:
		void Test() override;

		bool FormatToStringTest();
		bool SingleObjectToStringTest();
	};
}
#endif