#include "pch.hpp"
#include "PerformanceTestBase.hpp"

#include "PerformanceTestManager.hpp"

#include "../../Utility/Format/kFormatToString.hpp"

#ifdef TESTING_ENABLED
namespace kTest::performance
{
	PerformanceTestBase::PerformanceTestBase(const char* name) noexcept
		: name(name)
	{}

	PerformanceTestBase::~PerformanceTestBase()
		= default;

	void PerformanceTestBase::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (...)
		{
			const auto output = klib::kFormat::ToString("{0} seems to have crashed!\n", name);
			PerformanceTestManager::Get().CollectResult(output);
		}

		Output();
	}

	void PerformanceTestBase::Add(PerformanceTestBase* test)
	{
		PerformanceTestManager::Get().Add(test);
	}

	void PerformanceTestBase::SetUpParticipants(const std::vector<std::string_view>& participants) noexcept
	{
		std::string subTestName = participants.front().data();

		auto iter = participants.cbegin();
		++iter;
		for (; iter != participants.cend(); ++iter)
		{
			const auto name = *iter;
			subTestName.append(" vs ");
			subTestName.append(name.data());
		}

		iter = participants.cbegin();
		for (; iter != participants.cend(); ++iter)
			AddSubTest(subTestName, *iter);
	}

	void PerformanceTestBase::AddSubTest(const std::string& subTestName, const std::string_view& participant) noexcept
	{
		const auto& subTest = results[subTestName];

		if (subTest.find(subTestName) != subTest.end())
			return;

		results[subTestName][participant.data()] = AverageTime{ 0, 0 };
	}

	void PerformanceTestBase::Output() noexcept
	{
		for (auto& data : results)
		{
			data.second.erase("");

			const auto& subTestName = data.first;
			auto& internalData = data.second;

			std::pair<std::string, long long> fastestParticipant = { name, std::numeric_limits<long long>::max() };

			for (const auto& res : profilerResults)
			{
				if (internalData.find(res.name) == internalData.end())
					continue;

				const auto speed = res.end - res.start;
				auto& timePair = internalData[res.name];
				timePair.time += speed;
				timePair.count++;

				if (speed < fastestParticipant.second)
				{
					fastestParticipant.first = res.name;
					fastestParticipant.second = speed;
				}
			}

			auto minTime = std::numeric_limits<long double>::max();
			auto maxTime = std::numeric_limits<long double>::min();
			std::string winner;

			for (const auto& values : internalData)
			{
				const auto totalTime = values.second.time;
				const auto count = values.second.count;
				const long double avg = CAST(long double, totalTime) / count;

				const auto currentlowest = avg < minTime ? avg : minTime;

				if (currentlowest < minTime)
				{
					winner = values.first;
					minTime = currentlowest;
				}

				maxTime = avg > maxTime ? avg : maxTime;
			}

			const auto difference = maxTime - minTime;
			const auto percentage = (CAST(long double, difference) / minTime) * 100;
			const auto percentagef = CAST(float, percentage);

			SendResult(subTestName, winner, percentagef, fastestParticipant);
		}
	}

	void PerformanceTestBase::SendResult(const std::string_view& subTestName, const std::string_view& result, const float percentageDifference, const std::pair<std::string, long long>& fastestParticipant) noexcept
	{
		const auto output = klib::kFormat::ToString("%s: \n\t\"%s\" is the faster by %.3f%%\n\tFastest Run: %s Time: %d", 
			subTestName.data(),
			result.data(), 
			percentageDifference,
			fastestParticipant.first.data(), 
			fastestParticipant.second);
		PerformanceTestManager::Get().CollectResult(output);
	}

	const std::string& PerformanceTestBase::GetName() const
	{
		return name;
	}
}
#endif