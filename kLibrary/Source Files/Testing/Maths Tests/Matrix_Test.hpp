#pragma once
#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class MatricesTester final : public Tester
	{
	public:
		MatricesTester();
		~MatricesTester() override;

	private:
		void Test() override;

		bool DynamicMatrixTest();
		bool AddTest();
		bool SubtractTest();
		bool DivideTest();
		bool MultiplyTest();
		bool ConstexprTest();
	};
}
#endif