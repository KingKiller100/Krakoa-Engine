#include "pch.hpp"
#include "Fraction_Test.hpp"

#include "../../Maths/Constants.hpp"
#include "../../Maths/Fraction.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{

	FractionTester::FractionTester()
		: Tester("Fraction Test")
	{}

	FractionTester::~FractionTester()
		= default;

	using namespace kmaths;

	void FractionTester::Test()
	{
		VERIFY_MULTI_INIT();

		VERIFY_MULTI(AddTest);
		VERIFY_MULTI(SubractTest);
		VERIFY_MULTI(RationalTest);
		VERIFY_MULTI(IrrationalTest);

		VERIFY_MULTI_END();
	}

	double FractionTester::RoundTo4(double x)
	{
		const auto isNegative = x < 0;
		if (isNegative)
			x = -x;

		const auto accuracy = 1e3;
		const auto dpShifts = 1e-4 * 5;

		const auto valuePlusDpsByAcc = (x + dpShifts) * accuracy;
		const auto accuracyInverse = 1.0 / accuracy;
		const auto significantFigures = int(valuePlusDpsByAcc);
		const auto roundedValue = significantFigures * accuracyInverse;
		return isNegative ? -roundedValue : roundedValue;
	}

	bool FractionTester::AddTest()
	{
		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 1, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 5, true };
			auto res = f1 + f2;

			VERIFY(res.numerator == 2);
			VERIFY(res.denominator == 1);
			VERIFY(res.isNegative == false);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 1);
			VERIFY_COMPILE_TIME(res.denominator == 2);
			VERIFY_COMPILE_TIME(res.isNegative == false);
		}

		{
			constexpr Fraction f1 = { 12, 9, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 11);
			VERIFY_COMPILE_TIME(res.denominator == 6);
			VERIFY_COMPILE_TIME(res.isNegative == false);
		}

		{
			constexpr Fraction f1 = { 120, 45, false };
			constexpr Fraction f2 = { 180, 270, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 3);
			VERIFY_COMPILE_TIME(res.isNegative == false);
		}

		return success;
	}

	bool FractionTester::SubractTest()
	{
		return success;
	}


	bool FractionTester::RationalTest()
	{
		{
			constexpr Fraction f;
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 0);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<int>();
			VERIFY_COMPILE_TIME(f2r == 0);
		}

		{
			constexpr Fraction f = { 11, 2, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 11);
			VERIFY_COMPILE_TIME(f.denominator == 2);

			constexpr auto f2r = f.GetReal<float>();
			VERIFY_COMPILE_TIME(f2r == 5.5f);
		}

		{
			constexpr Fraction f = { 1000, 250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 4);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<float>();
			VERIFY_COMPILE_TIME(f2r == -4);
		}

		{
			constexpr Fraction f = { 1000, 250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 4);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<unsigned>();
			VERIFY(f2r == 0);
		}

		{
			constexpr Fraction f = { 625, 1250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 1);
			VERIFY_COMPILE_TIME(f.denominator == 2);

			constexpr auto f2r = f.GetReal<double>();
			VERIFY_COMPILE_TIME(f2r == -0.5);
		}

		return success;
	}

	bool FractionTester::IrrationalTest()
	{
		{
			constexpr Fraction f = { 333, 106, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 333);
			VERIFY_COMPILE_TIME(f.denominator == 106);

			constexpr auto f2r = f.GetReal<int>();
			VERIFY_COMPILE_TIME(f2r == 3);
		}

		{
			constexpr Fraction f = { 333, 106, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 333);
			VERIFY_COMPILE_TIME(f.denominator == 106);

			const auto f2r = RoundTo4(f.GetReal<double>());
			const auto pi = M_PI;
			const auto piRounded = RoundTo4(pi);
			VERIFY(f2r == RoundTo4(pi));
		}

		return success;
	}

}
#endif
