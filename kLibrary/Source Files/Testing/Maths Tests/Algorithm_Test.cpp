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
		VERIFY_MULTI(RoundingTest);
		VERIFY_MULTI(PowerOfTest);
		VERIFY_MULTI(SwapTest);
		VERIFY_MULTI(ToDegreesTest);
		VERIFY_MULTI(ToRadiansTest);
		VERIFY_MULTI(WhatPowerOf10Test);
		VERIFY_MULTI(InverseSquareRootTest);
		VERIFY_MULTI(SquareRootTest);
		VERIFY_MULTI(RootTest);
		VERIFY_MULTI(LogTest);

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
		VERIFY(intToDouble == 300.0);
		constexpr auto longDoubleToFloat = Convert<float>(longDoubleNum);
		VERIFY(longDoubleToFloat == CAST(float, longDoubleNum));
		constexpr auto longDoubleToInt = Convert<int>(longDoubleNum);
		VERIFY(longDoubleToInt == CAST(int, longDoubleNum));
		constexpr auto intToLongLong = Convert<long long>(maxInt);
		VERIFY(intToLongLong == maxInt);
		constexpr auto longLongToInt = Convert<int>(maxLongLong);
		VERIFY(longLongToInt == maxInt);

		// Non-primitives
		constexpr auto charToString_View = Convert<std::string_view>("YOLO");
		VERIFY(charToString_View == "YOLO");

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

		const auto doubleToVec = Convert<Vector4d>(5.0);
		VERIFY(doubleToVec.X() == 5.0 && doubleToVec.Y() == 5.0);

		return success;
	}

	bool AlgorithmsTester::MinMaxTest()
	{
		constexpr auto big = 1000LL;
		constexpr auto small = 2.55l;

		constexpr auto biggest = Max(big, small);
		VERIFY(biggest == big);
		constexpr auto biggestDouble = Max(small, big);
		VERIFY(biggestDouble == CAST(decltype(small), big));

		constexpr auto smallest = Min(small, big);
		VERIFY(smallest == small);
		constexpr auto smallestLongLong = Min(big, small);
		VERIFY(smallestLongLong == CAST(decltype(big), small));

		return success;
	}

	bool AlgorithmsTester::RoundingTest()
	{
		constexpr auto accuracy = 3;
		constexpr auto value = 1.0f / 128;
		const auto rounded = Round(value, accuracy);

		constexpr auto expectedVal = 0.008f;
		VERIFY(rounded == expectedVal);

		return success;
	}

	bool AlgorithmsTester::PowerOfTest()
	{
		{
			constexpr auto power = -4;
			constexpr auto num = 2.0f;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = std::powf(2, -4);
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 8;
			constexpr auto num = 2;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = std::powf(2, 8);
			VERIFY(value == expectedVal);
		}

		{
			constexpr auto power = 8;
			constexpr auto num = 25ll;
			constexpr auto value = PowerOf(num, power);
			const auto expectedVal = std::powf(25, 8);
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
		VERIFY(piTo180 == 180);

		constexpr auto tauTo360 = ToDegrees<float>(float(constants::TAU));
		VERIFY(tauTo360 == 360.f);

		constexpr auto piOver2To90 = ToDegrees<double>(constants::PI_OVER_2);
		VERIFY(piOver2To90 == 90.0);

		constexpr auto piOver4To45 = ToDegrees<long double>(constants::PI_OVER_4);
		VERIFY(piOver4To45 == 45.0l);

		constexpr auto pitimes3Over2To270 = ToDegrees<constants::AccuracyType>(constants::PI_OVER_2 * 3);
		VERIFY(pitimes3Over2To270 == 270.0l);

		return success;
	}

	bool AlgorithmsTester::ToRadiansTest()
	{
		constexpr auto deg180ToPi = ToRadians<int>(180);
		VERIFY(deg180ToPi == static_cast<decltype(deg180ToPi)>(M_PI));

		constexpr auto deg360ToTau = ToRadians<float>(360);
		VERIFY(deg360ToTau == static_cast<decltype(deg360ToTau)>(M_PI * 2));

		constexpr auto deg90ToPiOver2 = ToRadians<double>(90);
		VERIFY(deg90ToPiOver2 == static_cast<decltype(deg90ToPiOver2)>(M_PI_2));

		constexpr auto deg45ToPiOver4 = ToRadians<long double>(45);
		VERIFY(deg45ToPiOver4 == static_cast<decltype(deg45ToPiOver4)>(M_PI_4));

		constexpr auto deg270To3PiOver2 = ToRadians<long double>(270);
		VERIFY(deg270To3PiOver2 == static_cast<decltype(deg270To3PiOver2)>(M_PI_2 * 3));

		return success;
	}

	bool AlgorithmsTester::WhatPowerOf10Test()
	{
		{
			constexpr auto number = 0.1;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == -1);
		}

		{
			constexpr auto number = 1;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == 0);
		}

		{
			constexpr auto number = 10;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == 1);
		}
		{
			constexpr auto number = 300;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == 2);
		}

		{
			constexpr auto number = 30;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == 1);
		}

		{
			constexpr auto number = 300000;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == 5);
		}

		{
			constexpr auto number = 0.09567;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == -2);
		}

		{
			constexpr auto number = 0.00007;
			const auto powerOf10 = WhatPowerOf10(number);
			VERIFY(powerOf10 == -5);
		}

		return success;
	}

	bool AlgorithmsTester::SquareRootTest()
	{
		{
			constexpr auto square = 125348;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 354.045197 (as int)
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 100;
			const auto root = Sqrt(square);
			const auto expected = std::sqrtf(square); // 10
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
			constexpr auto square = 40694053.4567;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 6379.1890908406222
			VERIFY(root == expected);
		}

		{
			constexpr auto square = constants::PI;
			const auto root = Sqrt(square);
			const auto expected = std::sqrt(square); // 6379.1890908406222
			VERIFY(root == expected);
		}

		return success;
	}

	bool AlgorithmsTester::InverseSquareRootTest()
	{
		{
			constexpr auto square = 225;
			const auto root = InvSqrt<double>(square);
			const auto expected = std::sqrt(square); // 0.6666666666
			VERIFY(root == expected);
		}

		{
			constexpr auto square = 200.0f;
			const auto root = InvSqrt(square);
			const auto expected = std::sqrtf(square); // 0.0707106814
			VERIFY(root == expected);
		}


		return success;
	}

	bool AlgorithmsTester::RootTest()
	{
		{
			constexpr auto exponant = 8;
			constexpr auto power = 3;
			const auto root = Root(exponant, power);
			constexpr auto expected = 2;
			VERIFY(root == expected);
		}


		return success;
	}

	bool AlgorithmsTester::LogTest()
	{
		{
			constexpr auto base = 2;
			constexpr auto exponent = 8;
			const auto log = Log(base, exponent);
			const auto expected = std::log2(8); // 6379.1890908406222
			VERIFY(log == expected);
		}

		return success;
	}

}
#endif
