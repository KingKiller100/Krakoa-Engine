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
		VERIFY(ConvertTest() == true);
	}

	bool AlgorithmsTester::ConvertTest()
	{
		constexpr auto num = 300;
		constexpr auto maxInt = std::numeric_limits<int>::max();
		constexpr auto maxLongLong = std::numeric_limits<long long>::max();
		constexpr auto longLongNum = 10ll + maxInt;
		constexpr auto longDoubleNum = 25.672839239574873l;

		const auto intToDouble = Convert<double>(num);
		constexpr auto longDoubleTofloat = Convert<float>(longDoubleNum);
		constexpr auto longDoubleToInt = Convert<int>(longDoubleNum);
		constexpr auto intToLongLong = Convert<long long>(num);
		constexpr auto longLongToInt = Convert<int>(maxLongLong);

		constexpr auto charToString_View = Convert<std::string_view>("YOLO");
		constexpr int container[3][2] = { {1, 3}, {5, 6}, {} };
		const auto d = &container[0][1];
		constexpr auto mat = kmaths::IdentityMatrix<float, 2, 2>();
		//constexpr auto vecToMat = Convert<Matrix2x2d>(5.0);
		return false;
	}

}
#endif
