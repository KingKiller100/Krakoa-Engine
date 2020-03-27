#include <pch.hpp>
#include <Testing/Maths Tests/Vectors_Test.hpp>

#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Vectors/Vector4.hpp>

#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	using namespace kmaths;
	
	VectorsTester::VectorsTester()
		: Tester("Vectors 2/3/4 Test")
	{ }
	
	VectorsTester::~VectorsTester()
		= default;

	void VectorsTester::Test()
	{
		VERIFY(Vector2Test() == true);
		VERIFY(Vector3Test() == true);
		VERIFY(Vector4Test() == true);
	}

	bool VectorsTester::Vector2Test()
	{
	//	auto v2 = Vector2d(2.5, -11.0);
	//	VERIFY(v2.X() == 2.5 && v2.Y() == -11.0);
	//	
	//	v2.ToPositives();
	//	VERIFY(v2.X() > 0 || v2.Y() > 0);
	//	
	//	const auto xVal = v2.X();
	//	const auto yVal = v2.Y();
	//	v2.ReverseVector();
	//	VERIFY(xVal != v2.X() && yVal != v2.Y());

	//	auto g = v2 *= 8;
	//	VERIFY(g.X() == -20.0 && g.Y() == -88.0);
	//	
	//	g = v2 /= 8;
	//	VERIFY(g.X() == -2.5 && g.Y() == -11.0);

	//	v2 *= 10;
	//	v2.Truncate(30);
	//	const auto tempMag = v2.Magnitude();
	//	VERIFY(tempMag <= 30);
	//	
	//	const auto temp1 = v2.Perpendicular();
	//	const auto dist = v2.Distance(temp1);
	//	
	//	const auto temp2 = -v2;
	//	VERIFY(v2 != temp2);

	//	const auto temp3 = v2.Magnitude();

	//	v2.Zero();
	//	VERIFY(v2.Magnitude() == 0);

		return success;
	}

	bool VectorsTester::Vector3Test()
	{
	//	auto v3 = Vector3f(12.6f, 3.4f, 10.345f);
	//	VERIFY(v3.X() == 12.6f && v3.Y() == 3.4f && v3.Z() == 10.345f);

	//	const auto tempV3 = Vector3s(static_cast<int>(v3.Z()));
	//	VERIFY(tempV3.X() == static_cast<int>(v3.Z()) && tempV3.Y() == static_cast<int>(v3.Z()) && tempV3.Z() == static_cast<int>(v3.Z()));

	//	const auto v = v3.CrossProduct(tempV3);

	//	const auto ref = v3 [1];
	//	VERIFY(ref == 3.4f);

	//	// operator tests		
	//	auto result = tempV3 + v3;
	//	VERIFY(result.X() == 22 && result.Y() == 13 && result.Z() == 20);
	//	
	//	result = tempV3 * v3;
	//	VERIFY(result.X() == 120 && result.Y() == 30 && result.Z() == 100);
	//	
	//	result =  tempV3 - v3;
	//	VERIFY(result.X() == -2 && result.Y() == 7 && result.Z() == 0);
	//	
	//	result = tempV3 / v3;
	//	VERIFY(result.X() == tempV3.X() / static_cast<decltype(tempV3.X())>(v3.X()));
	//	VERIFY(result.Y() == tempV3.Y() / static_cast<decltype(tempV3.Y())>(v3.Y()));
	//	VERIFY(result.Z() == tempV3.Z() / static_cast<decltype(tempV3.Z())>(v3.Z()));
	//	
	//	v3 *= tempV3;
	//	v3 /= tempV3;
	//	v3 *= 2;
	//	v3 = tempV3;

		return success;
	}

	bool VectorsTester::Vector4Test()
	{
		/*const auto v4 = Vector4s(5, 7, 3, 1);

		const auto p = v4.W();
		VERIFY(p == 1);*/

		return success;
	}
}
#endif
