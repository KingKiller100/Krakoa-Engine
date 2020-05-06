#include "pch.hpp"
#include "Matrix_Test.hpp"

#include "../../Maths/Vectors/PredefinedVectors.hpp"
#include "../../Maths/Matrices/MatrixMathsHelper.hpp"
#include "../../Maths/Matrices/PredefinedMatrices.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	using namespace kmaths;

	MatricesTester::MatricesTester()
		: Tester("Matrix Test Dynamic/2x2/3x3/4x4")
	{	}

	MatricesTester::~MatricesTester()
		= default;

	void MatricesTester::Test()
	{
		VERIFY(DynamicMatrixTest() == true);
	}

	bool MatricesTester::DynamicMatrixTest()
	{
		constexpr auto m0 = Matrix<float, 2, 3>(1);
		constexpr auto m1 = Matrix<float, 3, 2>(1);
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
		//const auto m5 = m1 / m2; // Cannot compile due to division not being possible between matrices
		const auto m6 = m0 * m2;
		const auto m7 = m2 += m1;

		auto m8 = Matrix<float, 3, 3>(1);
		auto m9 = Matrix<float, 3, 10>(2);

		const auto m10 = m8 * m9;
		//const auto m11 = m8 / m9;
		constexpr auto m12 = Matrix<int, 5, 5>{
			{ 1, 2, 1, 0, 2 },
			{ 4, 11, 8, 0, 1 },
			{ 1, 6, 1, 0, 3 },
			{ 0, 0, 0, 6, 5 },
			{ 3, 5, 7, 6, 4 }
		};

		constexpr auto determinantM12 = m12.GetDeterminant();
		VERIFY(determinantM12 == -96);
		constexpr auto transposedM9 = m12.Transpose();
		for (auto row = 0u; row < m12.GetRows(); ++row)
			for (auto col = 0u; col < m12.GetColumns(); ++col)
				VERIFY(transposedM9[col][row] == m12[row][col]);

		{
			constexpr auto m13 = IdentityMatrix<float, 4, 4>();
			for (auto r = 0u; r < m13.GetRows(); ++r)
				for (auto c = 0u; c < m13.GetColumns(); ++c)
					VERIFY(m13[r][c] == (r == c ? 1 : 0));
		
			VERIFY(m13.IsIdentity());
		}

		const auto minorMatrix = m12.CreateMinorMatrix(0, 0);
		auto m14 = Matrix<double, 3, 3>();

		auto count = 0;
		for (auto r = 0u; r < m14.GetRows(); ++r)
			for (auto c = 0u; c < m14.GetColumns(); ++c)
				m14[r][c] = (c == 0 && r == 0) ? 10.0 : ++count;
		VERIFY(!m14.IsIdentity());

		constexpr auto size = m14.GetSize();

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

		const Vector<float, 3> vec{ 10, 10, 10 };
		const auto newVec = m0 * vec;
		VERIFY(newVec.X() == 30.f);
		VERIFY(newVec.Y() == 30.f);

		const auto newVec2 = m14 / vec;
		VERIFY(newVec2.X() == 0.f);
		VERIFY(newVec2.Y() == -10.f);
		VERIFY(newVec2.Z() == 10.f);
		
		// Currently constexpr supported functions for matrices

		constexpr Vector<float, 3> v{ 10, 10, 10 };
		constexpr auto m15 = Matrix<float, 3, 3>{
			{1, 2, 3},
			{3, 4, 6},
			{7, 8, 9},
		};

		constexpr auto m2v = m15 * v;
		VERIFY_COMPILE_TIME(m2v.X() == 60.f);
		VERIFY_COMPILE_TIME(m2v.Y() == 130.f);
		VERIFY_COMPILE_TIME(m2v.Z() == 240.f);

		constexpr auto m2v2 = m15 / v;
		VERIFY_COMPILE_TIME(m2v2.X() == -10.f);
		VERIFY_COMPILE_TIME(m2v2.Y() == 10.f);
		VERIFY_COMPILE_TIME(m2v2.Z() == 0.f);

		constexpr auto mirror = m15.Mirror();
		constexpr auto transpose = m15.Transpose();
		constexpr auto miniMatrix = m15.CreateMinorMatrix(0, 0);
		constexpr auto determinant = m15.GetDeterminant();
		constexpr auto inverse = m15.Inverse();
		constexpr auto isIdentity = m15.IsIdentity();
		constexpr auto numberOfElems = m15.GetSize();
		constexpr auto numberOfRows = m15.GetRows();
		constexpr auto numberOfColumns = m15.GetColumns();
		constexpr auto isSquare = m15.IsSquare();
		constexpr auto isZero = m15.IsZero();
		constexpr auto powerOfTwo = m15.PowerOf(2);
		// constexpr auto pointer = m15.GetPointerToData(); // Not supported til c++20

		constexpr auto dummyMat = Matrix<double, 3, 3>(1.0/128);
		constexpr auto add = m15 + dummyMat;
		constexpr auto multiple = m15 * dummyMat;
		constexpr auto scalarDiv = m15 / 2;
		constexpr auto subtract = m15 - dummyMat;
		constexpr auto scalarMul = m15 * 5;

		return success;
	}
}
#endif