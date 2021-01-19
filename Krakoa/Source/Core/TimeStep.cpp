#include "Precompile.hpp"
#include "TimeStep.hpp"

#include "../Debug/Instrumentor.hpp"

#include <Maths/kMathsFundamentals.hpp>

namespace krakoa::time
{
	using namespace klib::kStopwatch;

	namespace
	{
		void CalculateDeltaTime(AccurateStopwatch* sw, float target, bool isFixed, float multiplier, float* step) noexcept
		{
			KRK_PROFILE_FUNCTION();

			sw->Stop();

			const auto elapsed = sw->GetElapsedTime<units::Secs>();

			if (isFixed)
				*step =  kmaths::Min(elapsed, target) * multiplier;
			else
				*step = elapsed * multiplier;

			sw->Restart();
		}
	}

	TimeStep::TimeStep(const float targetMilliseconds) noexcept
		: step(0)
		, targetIncrement(1.f / targetMilliseconds)
		, speedMultiplier(1.f)
		, isFixedIncrement(true)
	{}

	TimeStep::~TimeStep() noexcept
	{
		stopwatch.Stop();
	}

	float TimeStep::GetLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return CalculateLifeTime();
	}

	float TimeStep::CalculateLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return stopwatch.GetLifeTime<units::Secs>();
	}

	float TimeStep::GetStep() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		CalculateDeltaTime(const_cast<AccurateStopwatch*>(std::addressof(stopwatch))
			, targetIncrement
			, isFixedIncrement
			, speedMultiplier
			, (float*)&step);
		return step;
	}

	void TimeStep::SetSpeedMultiplier(float speed) noexcept
	{
		speedMultiplier = speed;
	}
}
