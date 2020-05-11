#include "pch.hpp"
#include "Quaternion_Test.hpp"

#include "../../Maths/Matrices/Matrix3x4.hpp"
#include "../../Maths/Quaternions/Quaternions.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	QuaternionsTester::QuaternionsTester()
		: Tester("Quaternions Test")
	{}

	QuaternionsTester::~QuaternionsTester()
		= default;

	using namespace kmaths;

	template<typename T>
	const auto InRangeEpsilon = [](const T val) -> bool {
		constexpr auto one = kmaths::constants::One<T>();
		constexpr auto epsilon = std::numeric_limits<T>::epsilon();
		constexpr auto ub = one + epsilon;
		constexpr auto lb = one - epsilon;

		return ((val > lb) && (ub > val));
	};

	void QuaternionsTester::Test()
	{

		constexpr Matrix3x4f trianglePoints = {
			{ 2, 1, 0, 1 },
			{ 2, 5, 0, 1 },
			{ 4, 1, 0, 1 }
		};

		auto q1 = Quaternionf(90, 0, 0, 1);
		const auto mag = q1.MagnitudeSQ();
		VERIFY(InRangeEpsilon<float>(mag));

		//q1.Rotate(90.f, { 0, 0, 1 });
		const auto mag2 = q1.MagnitudeSQ();
		VERIFY(InRangeEpsilon<float>(mag2));


		auto q2 = Quaternionf(1, 0, 0, 0);
		const auto q3 = q1 * q2;

		constexpr auto decimalAccuracy = CAST(uint8_t, 2);

		{
			const auto matA = q3.CalculateTransformMatrix(trianglePoints[0]);
			const auto pointA = q3 * trianglePoints[0];
			const auto x = Round(pointA[0], decimalAccuracy);
			const auto y = Round(pointA[1], decimalAccuracy);

			VERIFY(x == -1.f);
			VERIFY(y == 2.f);
		}

		{
			const auto matB = q3.CalculateTransformMatrix(trianglePoints[1]);
			const auto pointB = q3 * trianglePoints[1];
			const auto x = Round(pointB[0], decimalAccuracy);
			const auto y = Round(pointB[1], decimalAccuracy);

			VERIFY(x == -5.f);
			VERIFY(y == 2.f);
		}

		{
			const auto matC = q3.CalculateTransformMatrix(trianglePoints[2]);
			const auto pointC = q3 * trianglePoints[2];
			const auto x = Round(pointC[0], decimalAccuracy);
			const auto y = Round(pointC[1], decimalAccuracy);

			VERIFY(x == -1.f);
			VERIFY(y == 4.f);
		}


		//constexpr Vector3f position{ 2, 4, 6 };
		//constexpr auto transformation = Matrix4x4f{
		//	{1, 2, 1, 2},
		//	{4, 11, 8, 4},
		//	{1, 6, 1, 6},
		//	{1, 6, 1, 8}
		//};
		//constexpr Quaternionf orientation(1, 2, 3, 4);
		//constexpr auto ans = orientation.CalculateTransformMatrix(position);



	}
}
#endif
