#pragma once

#include "../SpeedTestBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::speed::maths
{
	class AlgorithmsSpeedTest : public SpeedTestBase
	{
	public:
		AlgorithmsSpeedTest();
		~AlgorithmsSpeedTest();

	private:
		void Test() override;

		void PowerOfTest();
		void SqrtTest();
	};
}
#endif
