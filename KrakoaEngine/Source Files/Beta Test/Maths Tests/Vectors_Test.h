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
		
		bool Vector2Test();
		bool Vector3Test();
		bool Vector4Test();
	};
}
