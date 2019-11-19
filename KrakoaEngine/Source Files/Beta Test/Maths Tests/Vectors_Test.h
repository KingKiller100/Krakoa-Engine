#pragma once

#include "../Tester.h"

namespace kTest::Maths
{
	class VectorsTester : public Tester
	{
	public:
		VectorsTester();
		~VectorsTester() override;

	private:
		bool Test() override;
		
		bool Math_Vector2_Test();
		bool Math_Vector3_Test();
		bool Math_Vector4_Test();
	};
}
