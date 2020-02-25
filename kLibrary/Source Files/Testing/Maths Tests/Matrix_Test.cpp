#include <pch.hpp>
#include <Testing/Maths Tests/Matrix_Test.hpp>

#include <Maths/Matrices/Matrix4x4.hpp>


namespace kTest::Maths
{
	using namespace kMaths;

	MatricesTester::MatricesTester()
		: Tester("Matrix Test 2x2/3x3/4x4")
	{
	}

	MatricesTester::~MatricesTester()
	{
	}

	void MatricesTester::Test()
	{
		VERIFY(Matrix2x2Test() == true);
		VERIFY(Matrix3x3Test() == true);
		VERIFY(Matrix4x4Test() == true);
	}

	bool MatricesTester::Matrix2x2Test()
	{
		return true;
	}

	bool MatricesTester::Matrix3x3Test()
	{
		return true;
	}

	bool MatricesTester::Matrix4x4Test()
	{
		return true;
	}

}