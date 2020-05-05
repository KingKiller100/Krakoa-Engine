#pragma once

#include "../PerformanceTestBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::speed::maths
{
	class AlgorithmsSpeedTest : public PerformanceTestBase
	{
	public:
		AlgorithmsSpeedTest();
		~AlgorithmsSpeedTest();

	private:
		void Test() override;

		void PowerOfTest();
		void Square();
		void SqrtTest();
		void FloorTest();
		void FloatingPointRemainderTest();
	};
}
#endif
