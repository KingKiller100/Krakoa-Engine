#pragma once
#include <Testing/Tester.hpp>

#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	class MatricesTester : public Tester
	{
	public:
		MatricesTester();
		~MatricesTester() override;

	private:
		void Test() override;

		bool DynamicMatrixTest();
		bool Matrix2x2Test();
		bool Matrix3x3Test();
		bool Matrix4x4Test();
	};
}
#endif