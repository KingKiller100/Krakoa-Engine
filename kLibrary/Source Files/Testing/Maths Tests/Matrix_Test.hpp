#pragma once
#include "../Tester.hpp"

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
	};
}
#endif