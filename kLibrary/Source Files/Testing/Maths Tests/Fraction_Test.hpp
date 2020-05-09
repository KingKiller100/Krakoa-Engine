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
		bool MultiplyTest();
		bool DivideTest();
		bool NonSimplifiedTest();
		bool RationalTest();
		bool IrrationalTest();

	};
}
#endif