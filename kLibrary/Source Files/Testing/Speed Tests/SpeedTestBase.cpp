#include "pch.hpp"
#include "SpeedTestBase.hpp"

#include "SpeedTestManager.hpp"

#include "../../Utility/Format/kFormatToString.hpp"

#ifdef TESTING_ENABLED
namespace kTest::speed
{
	SpeedTestBase::SpeedTestBase(const char* name) noexcept
		: name(name)
	{}

	SpeedTestBase::~SpeedTestBase()
		= default;

	void SpeedTestBase::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (...)
		{
			const auto output = klib::kFormat::ToString("{0} seems to have crashed!\n", name);
			SpeedTestManager::Get().CollectResult(output);
		}

		Output();
	}

	void SpeedTestBase::SetUpParticipants(const std::vector<std::string_view>& participants) noexcept
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

	void SpeedTestBase::AddSubTest(const std::string& subTestName, const std::string_view& participant) noexcept
	{
		const auto& subTest = results[subTestName];

		if (subTest.find(subTestName) != subTest.end())
			return;

		results[subTestName][participant.data()] = AverageTime{ 0, 0 };
	}

	void SpeedTestBase::Output() noexcept
	{
		for (auto& data : results)
		{
			data.second.erase("");

			const auto& subTestName = data.first;
			auto& internalData = data.second;

			for (const auto& res : profilerResults)
			{
				if (internalData.find(res.name) == internalData.end())
					continue;

				const auto speed = res.end - res.start;
				auto& timePair = internalData[res.name];
				timePair.time += speed;
				timePair.count++;
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

			SendResult(subTestName, winner, percentagef);
		}
	}

	void SpeedTestBase::SendResult(const std::string_view& subTestName, const std::string_view& result, const float percentageDifference) noexcept
	{
		const auto output = klib::kFormat::ToString("%s: \"%s\" is the faster by %.3f%%\n", subTestName.data(), result.data(), percentageDifference);
		SpeedTestManager::Get().CollectResult(output);
	}

	const std::string& SpeedTestBase::GetName() const
	{
		return name;
	}
}
#endif