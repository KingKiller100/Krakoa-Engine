#pragma once

#include <Testing/Tester.hpp>

namespace kTest::Maths
{
	class MatricesTester : public Tester
	{
	public:
		MatricesTester();
		~MatricesTester() override;

	private:
		void Test() override;

		bool Matrix2x2Test();
		bool Matrix3x3Test();
		bool Matrix4x4Test();
	};
}