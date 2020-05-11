#include "pch.hpp"
#include "Quaternion_Test.hpp"

#include "../../Maths/Matrices/Matrix3x2.hpp"
#include "../../Maths/Quaternions/Quaternions.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	QuaternionsTester::QuaternionsTester()
		: Tester("Quaternions Test")
	{}

	QuaternionsTester::~QuaternionsTester()
		= default;

	void QuaternionsTester::Test()
	{
		using namespace kmaths;


		constexpr Matrix3x2f trianglePoints = {
			{ 2, 1 },
			{ 2, 5 },
			{ 4, 1 }
		};

		const auto q1 = Quaternionf(90, 0, 0, 1);
		auto q2 = Quaternionf(1, 0, 0, 0);
		q2 = q1 * q2;

		auto pointA = q2.CalculateTransformMatrix(trianglePoints[0]);
		auto pointB = q2.CalculateTransformMatrix(trianglePoints[1]);
		auto pointC = q2.CalculateTransformMatrix(trianglePoints[2]);
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
