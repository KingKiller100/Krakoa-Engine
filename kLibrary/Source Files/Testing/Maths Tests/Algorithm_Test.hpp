#pragma once
#include "../Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class AlgorithmsTester : public Tester
	{
	public:
		AlgorithmsTester();
		~AlgorithmsTester() override;

	private:
		void Test() override;

		bool ConversionTest();
		bool MinMaxTest();
		bool RoundingTest();
		bool PowerOfTest();
		bool SwapTest();
		bool ToDegreesTest();
		bool ToRadiansTest();
		bool SquareRootTest();
	};
}
#endif