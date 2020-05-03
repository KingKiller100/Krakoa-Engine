#include "pch.hpp"
#include "Algorithms_SpeedTest.hpp"

#include "../../../Maths/kAlgorithms.hpp"

#include <cmath>


#ifdef TESTING_ENABLED
namespace kTest::speed::maths
{
	AlgorithmsSpeedTest::AlgorithmsSpeedTest()
		: SpeedTestBase("Algorithms Speed Test")
	{}

	AlgorithmsSpeedTest::~AlgorithmsSpeedTest()
		= default;

	void AlgorithmsSpeedTest::Test()
	{
		PowerOfTest();
		SqrtTest();
	}

	void AlgorithmsSpeedTest::PowerOfTest()
	{
		AddSubTest("kmaths::PowerOf vs std::pow");

		constexpr long long set[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto size = kmaths::SizeOfArray(set);
		constexpr int valueBeforeIteration = 5;


		const auto myTestName = "kmaths::PowerOf";
		const auto stdTestName = "std::pow";

		auto power = 0;
		for (auto i = 0; i < 200; ++i)
		{
			power++;

			{
				PROFILE_SCOPE("kmaths::PowerOf");
			
				if (!(i % valueBeforeIteration))
					power;
				const auto idx = kmaths::Modulus<int>(i, size);
				const auto myResult = kmaths::PowerOf(set[idx], power);
			}

			{
				PROFILE_SCOPE("std::pow");
				
				if (!(i % valueBeforeIteration))
					power;

				const auto idx = kmaths::Modulus<int>(i, size);
				const auto stdResult = std::pow(set[idx], power);
			}
		}
	}

	void AlgorithmsSpeedTest::SqrtTest()
	{
		AddSubTest("kmaths::Sqrt vs std::sqrt");

		constexpr double set[] = { 0, 1e0, 2e1, 3e2, 4e3, 5e4, 6e5, 7e6, 8e7, 9e8 };
		constexpr auto size = kmaths::SizeOfArray(set);

		const auto myTestName = "kmaths::PowerOf";
		const auto stdTestName = "std::pow";

		auto power = 0.0;
		for (auto i = 0; i < 200; ++i)
		{
			power++;

			{
				PROFILE_SCOPE("kmaths::PowerOf");
				const auto idx = kmaths::Modulus<int>(i, size);
				const auto myResult = kmaths::Sqrt(set[idx]);
			}

			{
				PROFILE_SCOPE("std::pow");
				const auto idx = kmaths::Modulus<int>(i, size);
				const auto stdResult = std::sqrt(set[idx]);
			}
		}
	}
}
#endif
