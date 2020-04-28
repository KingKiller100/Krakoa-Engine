#include "pch.hpp"
#include "Vectors_Test.hpp"

#include "../../Maths/Vectors/VectorMathsHelper.hpp"
#include "../../Maths/Vectors/Vector2.hpp"
#include "../../Maths/Vectors/Vector3.hpp"
#include "../../Maths/Vectors/Vector4.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
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
		auto vec2d = Vector<2, double>{ 5, 12 };

		auto res1 = (vec2f + vec2d);
		VERIFY(res1.X() == 9.0 &&  res1.Y() == 16.0);
		res1 = vec2f - vec2d;
		VERIFY(res1.X() == -1.0 &&  res1.Y() == -8.0);
		res1 = vec2f * vec2d;
		VERIFY(res1.X() == 20 && res1.Y() == 48);
		res1 = vec2f / vec2d;
		VERIFY(res1.X() == 0.8f &&  res1.Y() == 1.f / 3.f);

		const auto distance = vec2f.Distance(vec2d);
		VERIFY(distance == 8.06225777f);
		vec2f = vec2f.Normalize();
		VERIFY(vec2f.X() == 0.707106769f && vec2f.Y() == 0.707106769f);

		constexpr auto length1 = Vector<3, double>(41, 55, 67);
		constexpr auto length2 = Vector< 3, double>(14, 55, 76);

		const auto angle1 = AngleBetweenVectors(length1, length2);
		const auto angle2 = AngleBetweenVectors(length1, length2, true);

		vec2d = vec2f;
		vec2f += vec2d;
		vec2f -= vec2d;
		vec2f *= vec2d;


		// Currently constexpr supported functions
		constexpr auto vec2s = Vector<2, int>(5, 12);
		constexpr auto vec2s2 = Vector<2, int>(2);
		constexpr Vector<3, int> copy = vec2s2;
		constexpr auto dp = vec2s.DotProduct(vec2s2);
		VERIFY(dp == 34);

		constexpr auto vec3s1 = Vector<3, int>(1, 2, 3);
		constexpr auto vec3s2 = Vector<3, int>(4, 5, 6);
		constexpr auto vec3l = Vector<3, long>(7, 8, 9);

		constexpr auto mag = vec3s1.MagnitudeSQ();
		VERIFY(mag == 14);

		VERIFY(angle2 == ToDegrees(angle1));
		VERIFY(angle1 == ToRadians(angle2));
		constexpr auto x = vec3s1.X();
		constexpr auto y = vec3s1.Y();
		constexpr auto z = vec3s1.Z();

		//const auto wFail = vec3s1.W(); // unable to compile this line as vector3 do not have W axis

		constexpr auto length2D = Vector2d(7, 16);
		constexpr auto perp = length2D.Perpendicular();
		VERIFY(static_cast<int>(perp.X()) == -16 && static_cast<int>(perp.Y()) == 7);
		//const auto perpFail = vec3s1.Perpendicular(); // unable to compile due to having more than only 2 axes
		constexpr auto crossP = vec3s2.CrossProduct(vec3l);
		VERIFY(crossP.X() == -3 && crossP.Y() == 6 && crossP.Z() == -3);
		//const auto crossPFail = vec2s.CrossProduct(vec2s); // unable to compile due to not having only 3 axes

		auto vec4u = Vector<4, unsigned>();
		constexpr char chars[] = { 'H', 'E', 'L', 'P', '\0' };
		constexpr auto vec5c = Vector<5, char>(chars);

		const auto w = vec4u.W();

		vec4u += vec3l;
		vec4u -= vec2d;

		constexpr auto vecULL = Vector<30, unsigned long long>(100);
		constexpr bool bools[] = { true, false, false, true, false, false, true };
		constexpr auto vecBool = Vector<7, bool>(bools);
		constexpr auto b6 = vecBool[6];

		constexpr auto lhs = Vector2s(10, 5);
		constexpr auto rhs = Vector2s(9, 15);

		constexpr auto res = lhs + rhs;
		constexpr auto res2 = lhs - rhs;
		constexpr auto res3 = lhs * rhs;
		constexpr auto res4 = lhs / rhs;
		constexpr auto res5 = lhs.ReverseVector();
		constexpr auto res6 = lhs.Inverse();
		constexpr auto res7 = lhs.IsZero();
		constexpr auto res8 = lhs.Perpendicular();
		constexpr auto res9 = lhs != rhs;

		return success;
	}
}
#endif
