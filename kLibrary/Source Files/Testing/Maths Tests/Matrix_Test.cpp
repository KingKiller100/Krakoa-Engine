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
		//m1.Identity(); Cannot compile since function is only usable for square matrix
		//m1.Inverse();	 Cannot compile since function is only usable for square matrix

		for (auto i = 0u; i < m0.GetRows(); ++i) {
			for (auto j = 0u; j < m0.GetColumns(); ++j)
			{
				VERIFY(m0[i][j] == 1);
				VERIFY(m1[j][i] == 1);
			}
		}

		auto m2 = Matrix<float, 3, 2>(2);
		const auto m3 = m1 - m2;
		//const auto m5 = m1 / 10; Cannot compile as division does not exist for matrices
		const auto m6 = m0 * m2;
		const auto m7 = m1 += m2;

		auto m8 = Matrix<float, 3, 3>(1);
		auto m9 = Matrix<float, 3, 10>(2);

		const auto m10 = m8 * m9;
		//const auto m11 = m8 / m9;
		auto m12 = Matrix<int, 5, 5>();

		m12[0] = MultiDimensionalVector<5, int>{ 1, 2, 1, 0, 2 };
		m12[1] = MultiDimensionalVector<5, int>{ 4, 11, 8, 0, 1 };
		m12[2] = MultiDimensionalVector<5, int>{ 1, 6, 1, 0, 3 };
		m12[3] = MultiDimensionalVector<5, int>{ 0, 0, 0, 6, 5 };
		m12[4] = MultiDimensionalVector<5, int>{ 3, 5, 7, 6, 4 };

		const auto determinantM12 = m12.GetDeterminant();
		const auto transposedM9 = m12.Transpose();
		const auto m13 = Matrix<size_t, 4, 4>::Identity();

		const auto inverse3x3 = m13.Inverse();

		for (auto i = 0u; i < m8.GetRows(); ++i)
			for (auto j = 0u; j < m8.GetColumns(); ++j)
				VERIFY(m8[i][j] == (i == j ? 1 : 0));

		return success;
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

		return success;
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

		return success;
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

		return success;
	}
}
#endif