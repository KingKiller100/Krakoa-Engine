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
		void Test() override;
		
		static void Math_Vector2_Test();
		static void Math_Vector3_Test();
		static void Math_Vector4_Test();
	};
}
