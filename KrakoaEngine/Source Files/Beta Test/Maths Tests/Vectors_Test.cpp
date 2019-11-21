#include "Precompile.h"
#include "Vectors_Test.h"

#include "../../Utility/Maths/Vectors/Vector2.h"
#include "../../Utility/Maths/Vectors/Vector3.h"
#include "../../Utility/Maths/Vectors/Vector4.h"

namespace kTest::Maths
{
	using namespace ::util::kMaths;
	
	VectorsTester::VectorsTester()
		: Tester("Vectors 2/3/4 Test")
	{}
	
	VectorsTester::~VectorsTester()
		= default;

	void VectorsTester::Test()
	{
		VERIFY(Math_Vector2_Test() == true);
		VERIFY(Math_Vector3_Test() == true);
		VERIFY(Math_Vector4_Test() == true);
	}

	bool VectorsTester::Math_Vector2_Test()
	{
		auto v2 = Vector2d(2.5, -11.0);
		VERIFY(v2.X() == 2.5 && v2.Y() == -11.0);
		
		v2.ToPositives();		
		VERIFY(v2.X() > 0 || v2.Y() > 0);
		
		const auto xVal = v2.X();
		const auto yVal = v2.Y();
		v2.ReverseVector();
		VERIFY(xVal != v2.X() && yVal != v2.Y());

		auto g = v2 *= 8;
		VERIFY(g.X() == 20.0 && g.Y() == -88.0);
		
		g = v2 /= 8;
		VERIFY(g.X() == -2.5 && g.Y() == -11.0);

		v2 *= 10;		
		v2.Truncate(30);
		const auto tempMag = v2.Magnitude();
		VERIFY(tempMag <= 30);
		
		const auto temp1 = v2.Perpendicular();
		v2.Distance(temp1);
		
		const auto temp2 = -v2;
		VERIFY(v2 != temp2) ;

		const auto temp3 = v2.Magnitude();

		v2.Zero();
		VERIFY(v2.Magnitude() == 0);

		return true;
	}

	bool VectorsTester::Math_Vector3_Test()
	{
		auto v3 = Vector3f(12.6f, 3.4f, 10.345f);
		VERIFY(v3.X() == 12.6f && v3.Y() == 3.4f && v3.Z() == 10.345f);

		const auto tempV3 = Vector3s(static_cast<int>(v3.Z()));
		VERIFY(tempV3.X() == v3.Z() && tempV3.Y() == v3.Z() && tempV3.Z() == v3.Z());

		auto v = CrossProduct(v3, tempV3);

		const auto ref = v3 [1];
		VERIFY(ref == 3.4f);

		// operator tests		
		auto result = tempV3 + v3;
		VERIFY(result.X() == 22 && result.Y() == 13 && result.Z() == 20);
		
		result = tempV3 * v3;
		VERIFY(result.X() == 126 && result.Y() == 34 && result.Z() == 103.45);
		
		result =  tempV3 - v3;
		VERIFY(result.X() == -2.6 && result.Y() == 6 && result.Z() == 0);
		
		result =  tempV3 / v3;
		VERIFY(result.X() == tempV3.X() / v3.X() && result.Y() == tempV3.Y() / v3.Y() && result.Z() == tempV3.Z() / v3.Z());
		
		v3 *= tempV3;
		v3 /= tempV3;
		v3 *= 2;
		v3 = tempV3;

		return true;
	}

	bool VectorsTester::Math_Vector4_Test()
	{
		const auto v4 = Vector4s(5, 7, 3, 1);

		const auto p = v4.W();
		VERIFY(p == 1);

		return true;
	}
}
