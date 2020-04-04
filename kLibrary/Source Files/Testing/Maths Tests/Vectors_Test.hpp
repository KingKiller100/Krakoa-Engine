#pragma once
#include "../Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	class VectorsTester : public Tester
	{
	public:
		VectorsTester();
		~VectorsTester() override;

	private:
		void Test() override;
		
		bool VectorTest();
		bool Vector2Test();
		bool Vector3Test();
		bool Vector4Test();
	};
}
#endif
