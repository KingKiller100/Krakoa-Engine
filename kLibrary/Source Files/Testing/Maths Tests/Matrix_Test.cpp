#include <pch.hpp>
#include <Testing/Maths Tests/Matrix_Test.hpp>

#include <Maths/Matrices/PredefinedMatrices.hpp>
#include <Maths/Vectors/PredefinedVectors.hpp>

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
		auto res = true;
		VERIFY(DynamicMatrixTest() == true);
		if (!success) res = false;
		success = true;
		VERIFY(Matrix2x2Test() == true);
		if (!success) res = false;
		success = true;
		VERIFY(Matrix3x3Test() == true);
		if (!success) res = false;
		success = true;
		VERIFY(Matrix4x4Test() == true);
		success = res;
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
		const auto m4 = m1 / 10;
		//const auto m5 = m1 / m2; Cannot compile due to division not being possible between matrices
		const auto m6 = m0 * m2;
		const auto m7 = m1 += m2;

		auto m8 = Matrix<float, 3, 3>(1);
		auto m9 = Matrix<float, 3, 10>(2);

		const auto m10 = m8 * m9;
		//const auto m11 = m8 / m9;
		auto m12 = Matrix<int, 5, 5>();

		m12[0] = Vector<5, int>{ 1, 2, 1, 0, 2 };
		m12[1] = Vector<5, int>{ 4, 11, 8, 0, 1 };
		m12[2] = Vector<5, int>{ 1, 6, 1, 0, 3 };
		m12[3] = Vector<5, int>{ 0, 0, 0, 6, 5 };
		m12[4] = Vector<5, int>{ 3, 5, 7, 6, 4 };

		const auto determinantM12 = m12.GetDeterminant();
		VERIFY(determinantM12 == -96);
		const auto transposedM9 = m12.Transpose();
		for (auto row = 0u; row < m12.GetRows(); ++row)
			for (auto col = 0u; col < m12.GetColumns(); ++col)
				VERIFY(transposedM9[col][row] == m12[row][col]);

		const auto m13 = Matrix<long long, 4, 4>::Identity();
		for (auto r = 0u; r < m13.GetRows(); ++r)
			for (auto c = 0u; c < m13.GetColumns(); ++c)
				VERIFY(m13[r][c] == (r == c ? 1 : 0));

		const auto minorMatrix = m12.CreateMinorMatrix(0, 0);
		auto m14 = Matrix<double, 3, 3>();

		auto count = 0;
		for (auto r = 0u; r < m14.GetRows(); ++r)
			for (auto c = 0u; c < m14.GetColumns(); ++c)
				m14[r][c] = (c == 0 && r == 0) ? 10.0 : ++count;

		const auto inverse3x3 = m14.Inverse();

		const auto identity1 = inverse3x3 * m14;
		const auto identity2 = m14 * inverse3x3;
		for (auto r = 0u; r < m14.GetRows(); ++r) {
			for (auto c = 0u; c < inverse3x3.GetColumns(); ++c)
			{
				VERIFY(identity1[r][c] == (r == c ? 1.0 : 0.0));
				VERIFY(identity2[r][c] == (r == c ? 1.0 : 0.0));
			}
		}

		return success;
	}

	bool MatricesTester::Matrix2x2Test()
	{
		auto m = Matrix2x2d();
		m[0] = Vector2d(1.0, 1);
		m[1] = Vector2d(2.0, 1);
		m.Identity();

		auto m2 = Matrix2x2d();
		m2[0] = Vector2d(1.0, 1);
		m2[1] = Vector2d(2.0, 1);

		const auto m3 = m - m2;
		const auto m5 = m / 10;

		return success;
	}

	bool MatricesTester::Matrix3x3Test()
	{
		auto m = Matrix3x3s();
		m[0] = Vector3u(1u, 2u, 3u);
		m[1] = Vector3u(2u, 3u, 4u);
		m[2] = Vector3u(3u, 5u, 6u);
		m.Identity();

		auto m2 = Matrix3x3s();
		m2[0] = Vector3u(1u, 2u, 3u);
		m2[1] = Vector3u(2u, 3u, 4u);
		m2[2] = Vector3u(3u, 5u, 6u);

		const auto m3 = m - m2;
		const auto m4 = m * m2;
		const auto m5 = m / 10;

		return success;
	}

	bool MatricesTester::Matrix4x4Test()
	{
		auto m = Matrix4x4f();
		m[0] = Vector4f(1.f, 1);
		m[1] = Vector4f(2.f, 1);
		m[2] = Vector4f(3.f, 1);
		m[3] = Vector4f(4.f, 1);
		m.Identity();

		auto m2 = Matrix4x4f();
		m2[0] = Vector4f(1.f, 1);
		m2[1] = Vector4f(2.f, 1);
		m2[2] = Vector4f(3.f, 1);
		m2[3] = Vector4f(4.f, 1);

		const auto m3 = m + m2;
		const auto m4 = m * m2;
		const auto m5 = m * 10;

		return success;
	}
}
#endif