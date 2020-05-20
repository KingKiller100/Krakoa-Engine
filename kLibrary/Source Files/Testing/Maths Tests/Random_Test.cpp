#include "pch.hpp"
#include "Random_Test.hpp"

#ifdef TESTING_ENABLED

#include "../../Maths/Length_Type.hpp"
#include "../../Maths/Random.hpp"

namespace kTest::maths
{
	RandomTester::RandomTester()
		: Tester("Random Number Tester")
	{}

	RandomTester::~RandomTester()
		= default;

	using namespace kmaths;
	void RandomTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(IntegerTest);
		VERIFY_MULTI(FloatingPointTest);
		VERIFY_MULTI_END();
	}

	bool RandomTester::IntegerTest()
	{
		constexpr Big_Int_Type lb = 0;
		constexpr Big_Int_Type ub = 100;

		const auto a = RNG(lb, ub);
		const auto b = RNG(lb, ub);

		VERIFY(a != b);
		VERIFY(lb < a && a < ub);
		VERIFY(lb < b && b < ub);

		return success;
	}

	bool RandomTester::FloatingPointTest()
	{
		constexpr float lb = 0.25f;
		constexpr float ub = 0.5f;

		const auto a = RNG(lb, ub);
		const auto b = RNG(lb, ub);

		VERIFY(a != b);
		VERIFY(lb < a && a < ub);
		VERIFY(lb < b && b < ub);

		return success;
	}
}
#endif


