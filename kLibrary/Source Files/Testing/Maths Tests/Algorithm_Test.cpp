#include "pch.hpp"
#include "Algorithm_Test.hpp"

#include "../../Maths/kAlgorithms.hpp"
#include "../../Maths/Vectors/PredefinedVectors.hpp"
#include "../../Maths/Matrices/PredefinedMatrices.hpp"

#include <numeric>
#include <string>

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	AlgorithmsTester::AlgorithmsTester()
		: Tester("Algorithms Test")
	{}

	AlgorithmsTester::~AlgorithmsTester()
		= default;

	void AlgorithmsTester::Test()
	{
		VERIFY_MULTI_INIT();

		VERIFY_MULTI(ConversionTest);
		VERIFY_MULTI(MinMaxTest);
		VERIFY_MULTI(FloorTest);
		VERIFY_MULTI(DecimalToFractionTest);
		VERIFY_MULTI(BinarySearchTest);
		VERIFY_MULTI(BinarySearchClosestTest);
		VERIFY_MULTI(PowerOfTest);
		VERIFY_MULTI(RoundingTest);
		VERIFY_MULTI(RoundingTest);
		VERIFY_MULTI(SwapTest);
		VERIFY_MULTI(ToDegreesTest);
		VERIFY_MULTI(ToRadiansTest);
		VERIFY_MULTI(WhatPowerOf10Test);
		VERIFY_MULTI(InverseSquareRootTest);
		VERIFY_MULTI(SquareRootTest);
		VERIFY_MULTI(RootTest);
		VERIFY_MULTI(LogTest);
		VERIFY_MULTI(PowerOfFractionTest);
		VERIFY_MULTI(FloatingPointRemainderTest);
		VERIFY_MULTI(ModulusTest);

		VERIFY_MULTI_END();
	}

	using namespace kmaths;
	bool AlgorithmsTester::ConversionTest()
	{
		constexpr auto num = 300;
		constexpr auto maxInt = std::numeric_limits<int>::max();
		constexpr auto maxLongLong = std::numeric_limits<long long>::max();
		constexpr auto longDoubleNum = 25.672839239574873l;

		// Primitives
		constexpr auto intToDouble = Convert<double>(num);
		VERIFY_COMPILE_TIME(intToDouble == 300.0);
		constexpr auto longDoubleToFloat = Convert<float>(longDoubleNum);
		VERIFY_COMPILE_TIME(longDoubleToFloat == CAST(float, longDoubleNum));
		constexpr auto longDoubleToInt = Convert<int>(longDoubleNum);
		VERIFY_COMPILE_TIME(longDoubleToInt == CAST(int, longDoubleNum));
		constexpr auto intToLongLong = Convert<long long>(maxInt);
		VERIFY_COMPILE_TIME(intToLongLong == maxInt);
		constexpr auto longLongToInt = Convert<int>(maxLongLong);
		VERIFY_COMPILE_TIME(longLongToInt == maxInt);

		// Non-primitives
		constexpr auto charToString_View = Convert<std::string_view>("YOLO");
		VERIFY_COMPILE_TIME(charToString_View == "YOLO");

		constexpr int container[3][2] = { {1, 3}, {5, 6}, {0, 5} };
		const auto arrayToMatrix = Convert<Matrix3x2s>(container);

		{
			const auto vecToMat = Convert<Matrix2x2f>(kmaths::Vector2f(5));

			for (auto row = 0; row < vecToMat.GetRows(); ++row)
				for (auto col = 0; col < vecToMat.GetColumns(); ++col)
					VERIFY(vecToMat[row][col] == (
					(row == col)
						? 5
						: 0)
					);
		}

		constexpr auto doubleToVec = Convert<Vector4d>(5.0);
		VERIFY_COMPILE_TIME(doubleToVec.X() == 5.0);
		VERIFY_COMPILE_TIME(doubleToVec.Y() == 5.0);
		VERIFY_COMPILE_TIME(doubleToVec.Z() == 5.0);
		VERIFY_COMPILE_TIME(doubleToVec.W() == 5.0);

		return success;
	}

	bool AlgorithmsTester::MinMaxTest()
	{
		constexpr auto big = 1000LL;
		constexpr auto small = 2.55l;

		constexpr auto biggest = Max(big, small);
		VERIFY_COMPILE_TIME(biggest == big);
		constexpr auto biggestDouble = Max(small, big);
		VERIFY_COMPILE_TIME(biggestDouble == CAST(decltype(small), big));

		constexpr auto smallest = Min(small, big);
		VERIFY_COMPILE_TIME(smallest == small);
		constexpr auto smallestLongLong = Min(big, small);
		VERIFY_COMPILE_TIME(smallestLongLong == CAST(decltype(big), small));

		return success;
	}

	bool AlgorithmsTester::FloorTest()
	{
		{
			constexpr auto decimal = 100.4756;
			constexpr auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = 50000000.48295;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = -0.48295;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = -1.48295;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = 1.48295;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = CAST(long double, std::numeric_limits<long long>::max()) + 11111.37859854;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		{
			constexpr auto decimal = CAST(long double, std::numeric_limits<long long>::max()) - 11111.37859854;
			const auto result = Floor(decimal);
			const auto expected = std::floor(decimal);
			VERIFY(result == expected);
		}

		return success;
	}

	bool AlgorithmsTester::DecimalToFractionTest()
	{
		{
			constexpr Fraction o = { 213, 109, false };
			constexpr auto ans = o.GetNumber<float>();
			constexpr auto decimal = 0.25;
			const auto fraction = DecimalToFraction<double>(decimal);
			VERIFY(fraction.GetSign() == 1 && fraction.GetNumerator() == 1 && fraction.GetDenominator() == 4);

			const auto f2d = fraction.GetNumber<decltype(decimal)>();
			VERIFY(f2d == decimal);
		}

		{
			constexpr auto decimal = 0.75;
			const auto fraction = DecimalToFraction(decimal);
			VERIFY(fraction.GetSign() == 1 && fraction.GetNumerator() == 3 && fraction.GetDenominator() == 4);

			const auto f2d = fraction.GetNumber<decltype(decimal)>();
			VERIFY(f2d == decimal);
		}

		{
			constexpr auto decimal = 10.75;
			const auto fraction = DecimalToFraction(decimal);
			VERIFY(fraction.GetSign() == 1 && fraction.GetNumerator() == 43 && fraction.GetDenominator() == 4);

			const auto f2d = fraction.GetNumber<decltype(decimal)>();
			VERIFY(f2d == decimal);
		}

		{
			constexpr auto decimal = -5.5f;
			const auto fraction = DecimalToFraction(decimal);
			VERIFY(fraction.GetSign() == -1 && fraction.GetNumerator() == 11 && fraction.GetDenominator() == 2);

			const auto f2d = fraction.GetNumber<decltype(decimal)>();
			VERIFY(f2d == decimal);
		}

		{
			constexpr double decimal = constants::PI;
			const auto fraction = DecimalToFraction(decimal);
			VERIFY(fraction.GetSign() == 1 && fraction.GetNumerator() == 312689 && fraction.GetDenominator() == 99532);

			const auto f2d = Round(fraction.GetNumber<decltype(decimal)>(), 3);
			VERIFY(f2d == Round(decimal, 3));
		}

		{
			constexpr double decimal = 14.568464;
			const auto fraction = DecimalToFraction(decimal);
			VERIFY(fraction.GetSign() == 1 && fraction.GetNumerator() == 910529 && fraction.GetDenominator() == 62500);

			const auto f2d = fraction.GetNumber<decltype(decimal)>();
			VERIFY(f2d == decimal);
		}

		return success;
	}

	bool AlgorithmsTester::BinarySearchTest()
	{
		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearch(list, 4.0);
			VERIFY_COMPILE_TIME(idx == 4);
		}
		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearch(list, -4.0);
			VERIFY_COMPILE_TIME(idx == -1);
		}

		{
			constexpr int list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearch(list, 14);
			VERIFY_COMPILE_TIME(idx == -1);
		}

		{
			auto list = new long long[7]{ 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearch(list, 6ll, 7);
			VERIFY(idx == 6);
		}

		return success;
	}

	bool AlgorithmsTester::BinarySearchClosestTest()
	{

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 1.0);
			VERIFY_COMPILE_TIME(idx == 1);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 3.0);
			VERIFY_COMPILE_TIME(idx == 3);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 5.0);
			VERIFY_COMPILE_TIME(idx == 5);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, -5.0);
			VERIFY_COMPILE_TIME(idx == -1);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 4.25);
			VERIFY_COMPILE_TIME(idx == 4);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 2.25);
			VERIFY_COMPILE_TIME(idx == 2);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 7.0);
			VERIFY_COMPILE_TIME(idx == -1);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, -1.0);
			VERIFY_COMPILE_TIME(idx == -1);
		}

		{
			constexpr double list[] = { 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 1.5);
			VERIFY_COMPILE_TIME(idx == 2);
		}

		{
			auto list = new long long[7]{ 0, 1, 2, 3, 4, 5, 6 };
			const auto idx = BinarySearchClosest(list, 6ll, 7);
			VERIFY(idx == 6);
		}

		return success;
	}

	bool AlgorithmsTester::RoundingTest()
	{
		{
			constexpr auto accuracy = 3;
			constexpr auto value = 1.0f / 128;
			constexpr auto rounded = Round(value, accuracy);

			constexpr auto expectedVal = 0.008f;
			VERIFY(rounded == expectedVal);
		}

		{
			constexpr auto accuracy = 0;
			constexpr auto value = 128.5;
			constexpr auto rounded = Round(value, accuracy);

			constexpr auto expectedVal = 129.0;
			VERIFY(rounded == expectedVal);
		}

		{
			constexpr auto accuracy = 0;
			constexpr auto value = -128.49;
			constexpr auto rounded = Round(value, accuracy);

			constexpr auto expectedVal = -128.0;
			VERIFY_COMPILE_TIME(rounded == expectedVal);
		}

		{
			constexpr auto accuracy = 1;
			constexpr auto value = 128.49;
			constexpr auto rounded = Round(value, accuracy);

			constexpr auto expectedVal = 128.5;
			VERIFY(rounded == expectedVal);
		}

		{
			constexpr auto accuracy = 4;
			constexpr auto value = 128.4939;
			constexpr auto rounded = Round(value, accuracy);

			const auto expectedVal = 128.4939;
			VERIFY(rounded == expectedVal);
		}

		return success;
	}

	bool AlgorithmsTester::PowerOfTest()
	{
		{
			constexpr auto power = -4.0f;
			constexpr auto num = 2.0f;
			const auto value = PowerOf(num, power);
			const auto expectedVal = std::powf(2, -4);
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 8.0;
			constexpr auto num = 2.0;
			const auto value = PowerOf(num, power);
			const auto expectedVal = std::powf(2, 8);
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 5;
			constexpr auto num = 4;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = CAST(decltype(value), std::powf(num, power));
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 2;
			constexpr auto num = 17;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = CAST(decltype(value), std::powf(num, power));
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 12;
			constexpr auto num = 7ll;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = CAST(decltype(value), std::pow(num, power));
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 4.0;
			constexpr auto num = 9.0;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = CAST(decltype(value), std::pow(num, power));
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 8;
			constexpr auto num = 25ll;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = CAST(decltype(value), std::pow(num, power));
			VERIFY(value == expectedVal);
		}

		return success;
	}

	bool AlgorithmsTester::SwapTest()
	{
		{
			auto thirty = 30;
			auto eight = 8;

			VERIFY(thirty == 30);
			VERIFY(eight == 8);

			Swap(thirty, eight);

			VERIFY(thirty == 8);
			VERIFY(eight == 30);
		}

		{
			std::string_view left = "right";
			std::string_view right = "left";

			VERIFY(left == "right");
			VERIFY(right == "left");

			Swap(left, right);

			VERIFY(left == "left");
			VERIFY(right == "right");
		}

		{
			std::unique_ptr<int> fiveHundred = std::make_unique<int>(200);
			std::unique_ptr<int> twoHundred = std::make_unique<int>(500);

			VERIFY(*fiveHundred == 200);
			VERIFY(*twoHundred == 500);

			Swap(fiveHundred, twoHundred);

			VERIFY(*fiveHundred == 500);
			VERIFY(*twoHundred == 200);
		}

		return success;
	}

	bool AlgorithmsTester::ToDegreesTest()
	{
		constexpr auto piTo180 = ToDegrees(int(constants::PI));
		VERIFY_COMPILE_TIME(piTo180 == 180);

		constexpr auto tauTo360 = ToDegrees<float>(float(constants::TAU));
		VERIFY_COMPILE_TIME(tauTo360 == 360.f);

		constexpr auto piOver2To90 = ToDegrees<double>(constants::PI_OVER_2);
		VERIFY_COMPILE_TIME(piOver2To90 == 90.0);

		constexpr auto piOver4To45 = ToDegrees<long double>(constants::PI_OVER_4);
		VERIFY_COMPILE_TIME(piOver4To45 == 45.0l);

		constexpr auto pitimes3Over2To270 = ToDegrees<constants::AccuracyType>(constants::PI_OVER_2 * 3);
		VERIFY_COMPILE_TIME(pitimes3Over2To270 == 270.0l);

		return success;
	}

	bool AlgorithmsTester::ToRadiansTest()
	{
		constexpr auto deg180ToPi = ToRadians<int>(180);
		VERIFY_COMPILE_TIME(deg180ToPi == static_cast<decltype(deg180ToPi)>(M_PI));

		constexpr auto deg360ToTau = ToRadians<float>(360);
		VERIFY_COMPILE_TIME(deg360ToTau == static_cast<decltype(deg360ToTau)>(M_PI * 2));

		constexpr auto deg90ToPiOver2 = ToRadians<double>(90);
		VERIFY_COMPILE_TIME(deg90ToPiOver2 == static_cast<decltype(deg90ToPiOver2)>(M_PI_2));

		constexpr auto deg45ToPiOver4 = ToRadians<long double>(45);
		VERIFY_COMPILE_TIME(deg45ToPiOver4 == static_cast<decltype(deg45ToPiOver4)>(M_PI_4));

		constexpr auto deg270To3PiOver2 = ToRadians<long double>(270);
		VERIFY_COMPILE_TIME(deg270To3PiOver2 == static_cast<decltype(deg270To3PiOver2)>(M_PI_2 * 3));

		return success;
	}

	bool AlgorithmsTester::WhatPowerOf10Test()
	{
		{
			constexpr auto number = 0.1;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == -1);
		}

		{
			constexpr auto number = 1;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == 0);
		}

		{
			constexpr auto number = 10;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == 1);
		}
		{
			constexpr auto number = 300;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == 2);
		}

		{
			constexpr auto number = 30;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == 1);
		}

		{
			constexpr auto number = 300000;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == 5);
		}

		{
			constexpr auto number = 0.09567;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == -2);
		}

		{
			constexpr auto number = 0.00007;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY_COMPILE_TIME(powerOf10 == -5);
		}

		return success;
	}

	bool AlgorithmsTester::SquareRootTest()
	{
		{
			constexpr auto square = 125348;
			const auto root = Sqrt(square);
			const auto expected = CAST(int, std::sqrtf(square)); // 354.045197 (as int)
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 100;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 10
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 2;
			const auto root = Sqrt<float>(square);
			const auto expected = std::sqrtf(square); // 1.141213...
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 3;
			const auto root = Sqrt<float>(square);
			const auto expected = std::sqrtf(square); // 1.7320508...
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 0;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 0
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 1;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 1
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 225;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 15
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 200.0f;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 14.1421356
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 4.0;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 2
			VERIFY(root == expected);
		}

		{
			constexpr auto square = .250;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 0.5
			VERIFY(root == expected);
		}

		{
			constexpr auto square = .00625;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 0.079056941504209485
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 40.f;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 6.424555
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 40694053.4567f;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 6379.1890908406222
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 27.f;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 5.....
			VERIFY(root == expected);
		}

		{
			constexpr auto square = CAST(float, constants::PI);
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 1.77245390...
			VERIFY(root == expected);
		}

		return success;
	}

	bool AlgorithmsTester::InverseSquareRootTest()
	{
		{
			constexpr auto square = 225;
			const auto invRoot = InvSqrt<double>(square);
			const auto root = 1.0 / invRoot;
			const auto expectedInv = 1.0 / std::sqrt(square); // 0.6666666666
			const auto expected = std::sqrt(square); // 15
			VERIFY(root == expected);
			VERIFY(invRoot == expectedInv);
		}

		{
			constexpr auto square = 200.0;
			const auto invRoot = InvSqrt(square);
			const auto root = 1.0 / invRoot;
			const auto expectedRoot = std::sqrt(square); // 14.1421356
			const auto expectedInv = 1.0 / expectedRoot; // 0.
			VERIFY(root == expectedRoot);
			VERIFY(invRoot == expectedInv);
		}

		return success;
	}

	bool AlgorithmsTester::RootTest()
	{
		{
			constexpr auto exponant = -1;
			constexpr auto power = 7;
			const auto root = Root(exponant, power);
			constexpr auto expected = -1;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 0;
			constexpr auto power = 14;
			const auto root = Root(exponant, power);
			constexpr auto expected = 0;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 1;
			constexpr auto power = 14;
			const auto root = Root(exponant, power);
			constexpr auto expected = 1;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 8;
			constexpr auto power = 4;
			const auto root = Root(exponant, power);
			constexpr auto expected = 1;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 1000.0l;
			constexpr auto power = 14;
			constexpr auto root = Root(exponant, power);
			constexpr auto expected = 1.6378937069540642l;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = .0625f;
			constexpr auto power = 4;
			constexpr auto root = Root(exponant, power);
			constexpr auto expected = 0.5;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 69150.l;
			constexpr auto power = 10;
			constexpr auto root = Root(exponant, power);
			constexpr auto expected = 3.0477491473803897l;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 27.0l;
			constexpr auto power = 2;
			const auto root = Root(exponant, power);
			const auto expected = std::sqrtl(exponant);
			VERIFY(root == expected);
		}

		{
			constexpr auto exponant = 8;
			constexpr auto power = 3;
			const auto root = Root(exponant, power);
			constexpr auto expected = 2;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 27;
			constexpr auto power = 3;
			const auto root = Root(exponant, power);
			constexpr auto expected = 3;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = -27;
			constexpr auto power = 3;
			const auto root = Root(exponant, power);
			constexpr auto expected = -3;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = -64;
			constexpr auto power = 3;
			constexpr auto root = Root(exponant, power);
			constexpr auto expected = -4;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = -200.l;
			constexpr auto power = 3;
			auto root = Root(exponant, power);
			constexpr auto expected = -5.8480354764257321l;
			VERIFY(root == expected);
		}

		{
			constexpr auto exponant = 50.f;
			constexpr auto power = 3;
			const auto root = Root(exponant, power);
			constexpr auto expected = 3.6840314986403859f;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 64.f;
			constexpr auto power = 3;
			constexpr auto root = Root(exponant, power);
			constexpr auto expected = 4;
			VERIFY_COMPILE_TIME(root == expected);
		}

		{
			constexpr auto exponant = 64.0;
			constexpr auto power = 6;
			auto root = Root(exponant, power);
			constexpr auto expected = 2;
			VERIFY(root == expected);
		}

		// Throws error trying to find a even root of a negative number
		//{
		//	constexpr auto exponant = -64.0;
		//	constexpr auto power = 6;
		//	auto root = RootImpl(exponant, power);
		//	constexpr auto expected = -2;
		//	VERIFY(root == expected);
		//}

		return success;
	}

	bool AlgorithmsTester::LogTest()
	{
		//{
		//	constexpr auto base = 2;
		//	constexpr auto exponent = 8;
		//	const auto log = Log(base, exponent);
		//	const auto expected = std::log2(8); // 6379.1890908406222
		//	VERIFY(log == expected);
		//}

		return success;
	}

	bool AlgorithmsTester::PowerOfFractionTest()
	{
		{
			constexpr auto numerator = 2;
			constexpr auto denominator = 3;
			constexpr auto base = 8;
			const auto result = PowerOf(base, numerator, denominator);
			constexpr auto expected = 4;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto numerator = 2;
			constexpr auto denominator = 4;
			constexpr auto base = 9;
			const auto result = PowerOf(base, numerator, denominator);
			constexpr auto expected = 3;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto numerator = 3;
			constexpr auto denominator = 5;
			constexpr auto base = 5.l;
			const auto result = PowerOf(base, numerator, denominator);
			constexpr auto expected = 2.6265278044037674l;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto decimal = 2.5;
			constexpr auto base = 5.0;
			const auto result = PowerOf(base, decimal);
			constexpr auto expected = 55.901699437494742l;
			VERIFY(result == expected);
		}

		return success;
	}

	bool AlgorithmsTester::FloatingPointRemainderTest()
	{
		{
			constexpr auto num = 11.1;
			constexpr auto base = 10.1;
			const auto result = FloatingPointRemainder(num, base);
			const auto expected = std::fmod(num, base);
			VERIFY(result == expected);
		}

		{
			constexpr auto num = 2.5f;
			constexpr auto base = 7.9f;
			const auto result = FloatingPointRemainder(num, base);
			const auto expected = std::fmod(num, base);
			VERIFY(result == expected);
		}

		{
			constexpr auto num = 101.1l;
			constexpr auto base = 10.0l;
			const auto result = FloatingPointRemainder(num, base);
			const auto expected = std::fmodl(num, base);
			VERIFY(result == expected);
		}

		{
			constexpr auto num = -10.4l;
			constexpr auto base = 10.0l;
			const auto result = FloatingPointRemainder(num, base);
			const auto expected = std::fmod(num, base);
			VERIFY(result == expected);
		}

		{
			constexpr auto num = 10.4l;
			constexpr auto base = 10.0l;
			const auto result = FloatingPointRemainder(num, base);
			const auto expected = std::fmod(num, base);
			VERIFY(result == expected);
		}

		return success;
	}

	bool AlgorithmsTester::ModulusTest()
	{
		{
			constexpr auto num = 10;
			constexpr auto base = 10;
			const auto result = Modulus(num, base);
			constexpr auto expected = num % base;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto num = 5;
			constexpr auto base = 10;
			const auto result = Modulus(num, base);
			constexpr auto expected = num % base;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto num = -46;
			constexpr auto base = 12;
			const auto result = Modulus(num, base);
			constexpr auto expected = num % base + base;
			VERIFY_COMPILE_TIME(result == expected);
		}

		{
			constexpr auto num = 46.6f;
			constexpr auto base = 12.2f;
			const auto result = Modulus(num, base);
			const auto expected = std::fmodf(num, base);
			VERIFY(result == expected);
		}

		return success;
	}

}
#endif
