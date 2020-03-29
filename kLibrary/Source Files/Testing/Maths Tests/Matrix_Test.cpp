#include <pch.hpp>
#include <Testing/Maths Tests/Matrix_Test.hpp>

#include <Maths/Matrices/Matrix.hpp>
#include <Maths/Matrices/Matrix2x2.hpp>
#include <Maths/Matrices/Matrix3x3.hpp>
#include <Maths/Matrices/Matrix4x4.hpp>

#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	using namespace kmaths;

	MatricesTester::MatricesTester()
		: Tester("Matrix Test Dynamic/2x2/3x3/4x4")
	{	}

	MatricesTester::~MatricesTester()
	{	}

	void MatricesTester::Test()
	{
		VERIFY(DynamicMatrixTest() == true);
		VERIFY(Matrix2x2Test() == true);
		VERIFY(Matrix3x3Test() == true);
		VERIFY(Matrix4x4Test() == true);
	}

	bool MatricesTester::DynamicMatrixTest()
	{
		auto m0 = Matrix<float, 2, 3>(1);
		auto m1 = Matrix<float, 3, 2>(1);
		m1.Identity();
		
		auto m2 = Matrix<float, 3, 2>(2);

		const auto m3 = m1 - m2;
		const auto m5 = m1 / 10;
		//const auto m6 = m1 * m0;
		const auto m7 = m1 += m2;

		return true;
	}

	bool MatricesTester::Matrix2x2Test()
	{
		auto m = Matrix2x2d();
		m._m1 = Vector2d(1.0, 1);
		m._m2 = Vector2d(2.0, 1);
		m.Identity();

		auto m2 = Matrix2x2d();
		m2._m1 = Vector2d(1.0, 1);
		m2._m2 = Vector2d(2.0, 1);

		const auto m3 = m - m2;
		const auto m4 = m / m2;
		const auto m5 = m / 10;

		return true;
	}

	bool MatricesTester::Matrix3x3Test()
	{
		auto m = Matrix3x3u();
		m._m1 = Vector3u(1u, 2u, 3u);
		m._m2 = Vector3u(2u, 3u, 4u);
		m._m3 = Vector3u(3u, 5u, 6u);
		m.Identity();

		auto m2 = Matrix3x3u();
		m2._m1 = Vector3u(1u, 2u, 3u);
		m2._m2 = Vector3u(2u, 3u, 4u);
		m2._m3 = Vector3u(3u, 5u, 6u);

		const auto m3 = m - m2;
		const auto m4 = m * m2;
		const auto m5 = m / 10;

		return true;
	}

	bool MatricesTester::Matrix4x4Test()
	{
		auto m = Matrix4x4f();
		m._m1 = Vector4f(1.f, 1);
		m._m2 = Vector4f(2.f, 1);
		m._m3 = Vector4f(3.f, 1);
		m._m4 = Vector4f(4.f, 1);
		m.Identity();

		auto m2 = Matrix4x4f();
		m2._m1 = Vector4f(1.f, 1);
		m2._m2 = Vector4f(2.f, 1);
		m2._m3 = Vector4f(3.f, 1);
		m2._m4 = Vector4f(4.f, 1);

		const auto m3 = m + m2;
		const auto m4 = m * m2;
		const auto m5 = m * 10;

		return true;
	}
}
#endif