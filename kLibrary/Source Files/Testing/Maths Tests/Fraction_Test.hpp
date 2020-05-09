#pragma once
#include "../Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class FractionTester : public Tester
	{
	public:
		FractionTester();
		~FractionTester() override;

	private:
		void Test() override;

		double RoundTo4(double x);
		
		bool AddTest();
		bool SubractTest();
		bool RationalTest();
		bool IrrationalTest();

	};
}
#endif