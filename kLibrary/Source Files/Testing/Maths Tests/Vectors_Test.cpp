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
		auto vec2f = Vector<float, 2>(4.f);
		auto vec2d = Vector<double, 2>{ 5, 12 };

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

		constexpr auto length1 = Vector<double, 3>(41, 55, 67);
		constexpr auto length2 = Vector<double, 3>(14, 55, 76);

		const auto rads = AngleBetweenVectors(length1, length2);
		const auto degs = AngleBetweenVectors(length1, length2, true);

		const auto toDegs = ToDegrees(rads);
		const auto toRads = ToRadians(degs);

		VERIFY(Round(degs, 16) == Round(toDegs, 16));
		VERIFY(Round(rads, 16) == Round(toRads, 16));

		vec2d = vec2f;
		vec2f += vec2d;
		vec2f -= vec2d;
		vec2f *= vec2d;

		const bool greaterThan = vec2d > 14;
		VERIFY(greaterThan == false);
		const bool lessThan = vec2d < 14;
		VERIFY(lessThan == true);
		
		// Currently constexpr supported functions
		constexpr auto vec2s = Vector<int, 2>(5, 12);
		constexpr auto vec2s2 = Vector<int, 2>(2);
		constexpr Vector<int, 3> copy = vec2s2;
		constexpr auto dp = vec2s.DotProduct(vec2s2);
		VERIFY(dp == 34);

		constexpr auto vec3s1 = Vector<int, 3>(1, 2, 3);
		constexpr auto vec3s2 = Vector<int, 3>(4, 5, 6);
		constexpr auto vec3l = Vector<long, 3>(7, 8, 9);

		constexpr auto magSq = vec3s1.MagnitudeSQ();
		VERIFY(magSq == 14);

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

		auto vec4u = Vector<unsigned, 4>();
		constexpr char chars[] = { 'H', 'E', 'L', 'P', '\0' };
		constexpr auto vec5c = Vector<char, 5>(chars);

		constexpr auto w = vec5c.W();

		vec4u += vec3l;
		vec4u -= vec2d;

		constexpr auto vecULL = Vector<unsigned long long, 30>(100);
		constexpr bool bools[] = { true, false, false, true, false, false, true };
		constexpr auto vecBool = Vector<bool, 7>(bools);
		constexpr auto b6 = vecBool[6];

		constexpr auto lhs = Vector2d(10, 5);
		constexpr auto rhs = Vector2d(9, 15);

		// Operators
		constexpr auto add = lhs + rhs;
		constexpr auto subtract = lhs - rhs;
		constexpr auto multiply = lhs * rhs;
		constexpr auto divide = lhs / rhs;
		constexpr auto noEqual = lhs != rhs;

		// Functions
		constexpr auto magnitudeSq = lhs.MagnitudeSQ();
		VERIFY_COMPILE_TIME(magnitudeSq == 125);
		constexpr auto magnitude = lhs.Magnitude();
		VERIFY_COMPILE_TIME(magnitude == 11.180339887498949);
		constexpr auto normalize = lhs.Normalize();
		constexpr auto dist = lhs.Distance(rhs);
		constexpr auto dotProduct = lhs.DotProduct(rhs);
		VERIFY_COMPILE_TIME(dotProduct == 165);
		constexpr auto dimensions = lhs.GetLength();
		VERIFY_COMPILE_TIME(dimensions == 2);
		constexpr auto reverseVector = lhs.ReverseVector();
		VERIFY_COMPILE_TIME(reverseVector.X() == -10 && reverseVector.Y() == -5);
		constexpr auto inverse = lhs.Inverse();
		VERIFY_COMPILE_TIME(inverse.X() == 0.1 && inverse.Y() == 0.2);
		constexpr auto isZero = lhs.IsZero();
		VERIFY_COMPILE_TIME(isZero == false);
		constexpr auto perpendicular = lhs.Perpendicular();
		VERIFY_COMPILE_TIME(perpendicular.X() == -5 && perpendicular.Y() == 10);

		constexpr auto lhs3d = Vector3d(10, 5, 3);
		constexpr auto rhs3d = Vector3d(9, 15, 23);

		constexpr auto crossProduct = lhs3d.CrossProduct(rhs3d);

		constexpr auto compareVal = 15;
		constexpr bool greater = (lhs3d >= compareVal);
		constexpr bool less = rhs3d <= 14;
		
		return success;
	}
}
#endif
