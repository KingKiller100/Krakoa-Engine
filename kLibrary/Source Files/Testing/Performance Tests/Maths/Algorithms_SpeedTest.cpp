#include "pch.hpp"
#include "Algorithms_SpeedTest.hpp"

#include "../../../Maths/kAlgorithms.hpp"

#include <cmath>
#include <memory>
#include <functional>


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	AlgorithmsSpeedTest::AlgorithmsSpeedTest()
		: PerformanceTestBase("Algorithms Speed Test")
	{}

	AlgorithmsSpeedTest::~AlgorithmsSpeedTest()
		= default;

	void AlgorithmsSpeedTest::Test()
	{
		PowerOfTest();
		SquareTest();
		SqrtTest();
		FloorTest();
		FloatingPointRemainderTest();
		SignTest();
		AbsTest();
	}

	constexpr auto maxIter = int(2e4);

	void AlgorithmsSpeedTest::PowerOfTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::PowerOf", "std::pow" };
		SetUpParticipants(participants);

		constexpr double set[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto size = kmaths::SizeOfCArray(set);
		constexpr int valueBeforeIteration = 1000;

		auto power = 0.0;
		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			if (!(i % valueBeforeIteration))
				power += 1;

			{
				START_TEST(participants[0]);
				const auto value = kmaths::PowerOf(number, power);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::pow(number, power);
			}
		}
	}

	void AlgorithmsSpeedTest::SquareTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Sqaure", "std::pow" };
		SetUpParticipants(participants);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto value = kmaths::Square(i);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::pow(i, 2);
			}
		}
	}

	void AlgorithmsSpeedTest::SqrtTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Sqrt", "std::sqrt", "kmaths::Root" };
		SetUpParticipants(participants);

		constexpr double set[] = { 0, 1e0, 2e1, 3e2, 4e3, 5e4, 6e5, 7e6, 8e7, 9e8 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Sqrt(number);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::sqrt(number);
			}

			{
				START_TEST(participants[2]);
				const auto value = kmaths::Root(number, 2);
			}
		}
	}

	void AlgorithmsSpeedTest::FloorTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Floor", "std::floor" };
		SetUpParticipants(participants);

		constexpr float set[] = { 0.2443f, 1.234543f, 2.7687f, 3.2342324f, 4.324543f, 5.5f, 6.354f, 7.234244554f, 8.4365f, 9.45434f };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Floor(number);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::floor(number);
			}
		}
	}
	void AlgorithmsSpeedTest::FloatingPointRemainderTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::FloatingPointRemainder", "std::fmod" };
		SetUpParticipants(participants);

		constexpr float set[] = { 0.2443f, 1.234543f, 2.7687f, 3.2342324f, 4.324543f, 5.5f, 6.354f, 7.234244554f, 8.4365f, 9.45434f };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto base = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::FloatingPointRemainder(CAST(float, i), base);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::fmod(CAST(float, i), base);
			}
		}
	}

	void AlgorithmsSpeedTest::SignTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Sign", "std::copysign" };
		SetUpParticipants(participants);

		constexpr float set[] = { -1, 0, 1 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto num = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Sign(num);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::copysign(num, num);
			}
		}
	}

	void AlgorithmsSpeedTest::AbsTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Abs", "std::fabsl" };
		SetUpParticipants(participants);

		constexpr long double set[] = { -71, -1, 0, 1, 349 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto num = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Abs(num);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::fabsl(num);
			}
		}
	}

}
#endif
