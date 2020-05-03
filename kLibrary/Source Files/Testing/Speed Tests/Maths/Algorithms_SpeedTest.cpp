#include "pch.hpp"
#include "Algorithms_SpeedTest.hpp"

#include "../../../Maths/kAlgorithms.hpp"

#include <cmath>


#ifdef TESTING_ENABLED
namespace kTest::speed::maths
{
	using namespace kmaths;

	AlgorithmsSpeedTest::AlgorithmsSpeedTest()
		: SpeedTestBase("Algorithms Speed Test")
	{}

	AlgorithmsSpeedTest::~AlgorithmsSpeedTest()
		= default;

	void AlgorithmsSpeedTest::Test()
	{
		PowerOfTest();
	}

	void AlgorithmsSpeedTest::PowerOfTest()
	{
		constexpr long long set[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto size = SizeOfArray(set);

		const auto myTestName = "kmaths::PowerOf";
		const auto stdTestName = "std::pow";

		auto power = 0;
		for (auto i = 0; i < 100; ++i)
		{
			{
				PROFILE_SCOPE("kmaths::PowerOf");
				if (!(i % 4))
					power++;

				const auto myResult = PowerOf(set[Modulus<size_t>(i, size)], power);
			}

			{
				PROFILE_SCOPE("std::pow");
				if (!(i % 4))
					power++;

				const auto stdResult = std::pow(set[Modulus<size_t>(i, size)], power);
			}
		}

		long double myAvg = 0;
		long double stdAvg = 0;

		for (const auto& result : profilerResults)
		{
			if (result.name == myTestName)
				myAvg += (result.end - result.start);
			else
				stdAvg += (result.end - result.start);
		}

		const auto halfTotalResults = (profilerResults.size() * 0.5l);
		myAvg /= halfTotalResults;
		stdAvg /= halfTotalResults;

		const auto winner = Min(myAvg, stdAvg) == myAvg ? myTestName : stdTestName;

		SendResult(winner);

	}
}
#endif
