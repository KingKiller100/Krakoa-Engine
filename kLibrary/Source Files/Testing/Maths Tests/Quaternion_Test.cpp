#include "pch.hpp"
#include "Quaternion_Test.hpp"

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

		constexpr Vector3f position{ 2, 4, 6 };

		constexpr auto transformation = Matrix4x4f{
			{1, 2, 1, 2},
			{4, 11, 8, 4},
			{1, 6, 1, 6},
			{1, 6, 1, 8}
		};

		constexpr Quaternionf orientation(1, 2, 3, 4);
		constexpr auto ans = orientation.CalculateTransformMatrix(position);



	}
}
#endif
