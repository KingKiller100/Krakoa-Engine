#include "pch.hpp"
#include "Vectors_Test.hpp"

#include "../../Maths/Vectors/VectorMathsHelper.hpp"
#include "../../Maths/Vectors/Vector2.hpp"
#include "../../Maths/Vectors/Vector3.hpp"
#include "../../Maths/Vectors/Vector4.hpp"

#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	using namespace kmaths;

	VectorsTester::VectorsTester()
		: Tester("Vectors X/2/3/4 Test")
	{ }

	VectorsTester::~VectorsTester()
		= default;

	void VectorsTester::Test()
	{
		VERIFY(VectorTest() == true);
	}

	bool VectorsTester::VectorTest()
	{
		auto vec2f = Vector<2, float>(4.f);
		auto vec2d = Vector<2, double>({ 5, 12 });

		auto res1 = vec2f + vec2d;
		VERIFY(res1.X() == 9.0 &&  res1.Y() == 16.0);
		res1 = vec2f - vec2d;
		VERIFY(res1.X() == -1.0 &&  res1.Y() == -8.0);
		res1 = vec2f * vec2d;
		VERIFY(res1.X() == 20 &&  res1.Y() == 48);
		res1 = vec2f / vec2d;
		VERIFY(res1.X() == 0.8f &&  res1.Y() == 1.f/3.f);

		const auto distance = vec2f.Distance(vec2d);
		VERIFY(distance == 8.06225777f);
		vec2f = vec2f.Normalize();
		VERIFY(vec2f.X() == 0.707106769f && vec2f.Y() == 0.707106769f);
		const auto vec2s = Vector<2, int>(2);
		const auto dp = vec2s.DotProduct(vec2d);
		VERIFY(dp == 34);

		vec2d /= vec2f;
		vec2f += vec2d;
		vec2f -= vec2d;
		vec2f *= vec2d;

		const auto vec3s1 = Vector<3, int>{ 1,2,3 };
		const auto vec3s2 = Vector<3, int>{ 4,5,6 };
		const auto vec3l = Vector<3, long>{ 7,8,9 };

		auto angle1 = AngleBetweenVectors(Vector<3, double>{ 41, 55, 67 }, Vector<3, double>{ 14, 55, 76 });
		auto angle2 = AngleBetweenVectors(Vector<3, double>{ 41, 55, 67 }, Vector<3, double>{ 14, 55, 76 }, true);
		
		VERIFY(angle2 == RadiansToDegrees(angle1));
		VERIFY(angle1 == DegreesToRadians(angle2));

		const auto x = vec3s1.X();
		const auto y = vec3s1.Y();
		const auto z = vec3s1.Z();
		//const auto wFail = vec3.W(); unable to compile this line as vector3 do not have W axis
		const auto perp = vec2d.Perpendicular();
		VERIFY(static_cast<int>(perp.X()) == -16 && static_cast<int>(perp.Y()) == 7);
		//const auto perpFail = vec3.Perpendicular(); unable to compile due to having more than only 2 axes
		const auto crossP = vec3s2.CrossProduct(vec3l);
		VERIFY(crossP.X() == -3 && crossP.Y() == 6 && crossP.Z() == -3);
		//const auto crossPFail = vec2a.CrossProduct(vec2b); unable to compile due to not having only 3 axes

		auto vec4u = Vector<4, unsigned>{};
		const auto vec5c = Vector<5, char>{ 'H', 'E', 'L', 'P', '\0' };

		const auto w = vec4u.W();

		vec4u += vec3l;
		vec4u -= vec2d;

		const auto vecULL = Vector<30, unsigned long long>(100);
		const auto vecBool = Vector<7, bool>({true, false, false, true, false, false, true});

		const auto b6 = vecBool[6];

		return success;
	}
}
#endif
