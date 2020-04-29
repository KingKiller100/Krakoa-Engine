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
	using namespace kmaths;

	AlgorithmsTester::AlgorithmsTester()
		: Tester("Algorithms Test")
	{

	}

	AlgorithmsTester::~AlgorithmsTester()
	{

	}

	void AlgorithmsTester::Test()
	{
		auto noFails = true;
		
		VERIFY(ConversionTest() == true);
		if (!success) noFails = false;
		success = true;
		
		VERIFY(MinMaxTest() == true);
		if (!success) noFails = false;
		success = true;
		
		VERIFY(RoundingTest() == true);
		if (!success) noFails = false;
		success = true;

		VERIFY(PowerOfTest() == true);
		if (!success) noFails = false;
		success = true;

		VERIFY(SwapTest() == true);
		if (!success) noFails = false;
		success = true;

		success = noFails;
	}

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
		VERIFY(charToString_View.compare("YOLO") == 0);

		constexpr int container[3][2] = { {1, 3}, {5, 6}, {0, 5} };
		constexpr auto cont = Convert<Matrix3x2s>(container);
		constexpr auto vecToMat = Convert<Matrix2x2f>(kmaths::Vector2f(5));
		for (auto row = 0; row < vecToMat.GetRows(); ++row)
			for (auto col = 0; col < vecToMat.GetColumns(); ++col)
				VERIFY(vecToMat[row][col] == (
				(row == col) 
					? 5
					: 0)
				);
		
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
		constexpr auto value = 1.0 / 32;
		constexpr auto v = 0.030000000;
		constexpr auto rounded = Round(value, accuracy);

		return success;
	}

	bool AlgorithmsTester::PowerOfTest()
	{
		constexpr auto power = -4;
		constexpr auto num = 2.0f;

		constexpr auto value = PowerOf(num, power);

		return success;
	}
	bool AlgorithmsTester::SwapTest()
	{
		auto thirty = 30;
		auto eight = 8;

		VERIFY(thirty == 30);
		VERIFY(eight == 8);

		Swap(thirty, eight);

		VERIFY(thirty == 8);
		VERIFY(eight == 30);

		return success;
	}
}
#endif
