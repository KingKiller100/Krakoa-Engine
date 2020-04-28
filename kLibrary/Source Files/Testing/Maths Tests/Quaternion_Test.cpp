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

		Vector3f position;
		
		for (auto i = 0u; i < position.NumberOfDimensions(); ++i)
			position[i] = CAST(float, (i + 1) * 2);

		auto transformation = Matrix4x4d();

		transformation[0] = {1, 2, 1, 2};
		transformation[1] = {4, 11, 8, 4};
		transformation[2] = {1, 6, 1, 6};
		transformation[3] = {1, 6, 1, 8};

		Quaterniond orientation(1,2,3,4);
		orientation.CalculateTransformMatrix(transformation, position);
		
		

	}
}
#endif
