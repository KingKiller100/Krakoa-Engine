#pragma once

#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class UTFConverterTester : public Tester
	{
	public:
		UTFConverterTester();
		~UTFConverterTester() override;

	private:
		void Test() override;
	};
}
#endif
