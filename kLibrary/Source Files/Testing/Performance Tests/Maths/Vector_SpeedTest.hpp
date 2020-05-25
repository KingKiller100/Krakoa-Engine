#pragma once

#include "../PerformanceTestBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	class VectorSpeedTest : public PerformanceTestBase
	{
	public:
		explicit VectorSpeedTest();
		~VectorSpeedTest() override;

	protected:
		void Test() override;

	private:
		void Vec2f();
		void Vec3f();
		void Vec4f();
	};
}
#endif
