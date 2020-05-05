#pragma once

#include "../PerformanceTestBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	class AlgorithmsSpeedTest : public PerformanceTestBase
	{
	public:
		AlgorithmsSpeedTest();
		~AlgorithmsSpeedTest();

	private:
		void Test() override;

		void PowerOfTest();
		void SquareTest();
		void SignTest();
		void AbsTest();
		void SqrtTest();
		void FloorTest();
		void FloatingPointRemainderTest();
	};
}
#endif
