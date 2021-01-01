#include "Precompile.hpp"
#include "TimeStep.hpp"

#include <Maths/kMathsFundamentals.hpp>

namespace krakoa::time
{
	using namespace klib::kStopwatch;

	namespace
	{
		float CalculateDeltaTime(AccurateStopwatch* sw, float target, bool isFixed) noexcept
		{
			KRK_PROFILE_FUNCTION();

			sw->Stop();
			
			const auto elapsed = sw->GetElapsedTime<units::Secs>();

			sw->Restart();
			
			if (isFixed)
				return kmaths::Min(elapsed, target);
			else
				return elapsed;
		}
	}

	TimeStep::TimeStep(const float targetMilliseconds) noexcept
		: targetIncrement(1.f / targetMilliseconds),
		isFixedIncrement(true)
	{}

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

	float TimeStep::GetDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return CalculateDeltaTime(const_cast<AccurateStopwatch*>(std::addressof(stopwatch))
			, targetIncrement
			, isFixedIncrement);
	}
}
