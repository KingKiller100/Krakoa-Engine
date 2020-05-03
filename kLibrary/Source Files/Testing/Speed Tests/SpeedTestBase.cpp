#include "pch.hpp"
#include "SpeedTestBase.hpp"

#include "SpeedTestManager.hpp"

#include "../../Utility/Format/kFormatToString.hpp"

#include "../../Maths/kAlgorithms.hpp"

#include <map>

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

		GetFastestTime();
	}

	void SpeedTestBase::AddSubTest(const std::string& subTestName) noexcept
	{
		if (results.find(subTestName) != results.end())
			return;

		results[subTestName][""] = AverageTime{ 0, 0 };
	}

	void SpeedTestBase::GetFastestTime() noexcept
	{
		for (auto& data : results)
		{
			const auto& subTest = data.first;
			auto& internalData = data.second;

			for (const auto& res : profilerResults)
			{
				const auto speed = res.end - res.start;

				if (internalData.find(res.name) == internalData.end())
				{
					internalData[res.name] = { 0, 0 };
				}

				auto& timePair = internalData[res.name];
				timePair.time += speed;
				timePair.count++;
			}

			auto winTime = std::numeric_limits<long long>::max();
			std::string_view winner;

			for (const auto& values : internalData)
			{
				const auto totalTime = values.second.time;
				const auto count = values.second.count;
				const auto avg = totalTime / count;

				const auto minTime = kmaths::Min(winTime, avg);

				if (minTime < winTime)
				{
					winner = values.first;
					winTime = minTime;
				}
			}

			SendResult(subTest, winner);
		}
	}

	void SpeedTestBase::SendResult(const std::string_view& subTestName, const std::string_view& result) noexcept
	{
		const auto output = klib::kFormat::ToString("{0}: \"{1}\" is the winner!\n", subTestName, result);
		SpeedTestManager::Get().CollectResult(output);
	}

	const std::string& SpeedTestBase::GetName() const
	{
		return name;
	}
}
#endif