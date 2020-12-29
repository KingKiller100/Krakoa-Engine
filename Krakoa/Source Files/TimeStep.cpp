#include "Precompile.hpp"
#include "TimeStep.hpp"

#include <Utility/Stopwatch/kStopwatch.hpp>
#include <Maths/kMathsFundamentals.hpp>

namespace krakoa::time
{
	klib::kStopwatch::AccurateStopwatch timer;

	TimeStep::TimeStep(const float targetMilliseconds) noexcept
		: targetIncrement(1.f / targetMilliseconds),
		isTimeIncrementFixed(true)
	{}

	float TimeStep::GetLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return CalculateLifeTime();
	}

	float TimeStep::CalculateLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return timer.GetLifeTime<klib::kStopwatch::units::Secs>();
	}

	float TimeStep::GetDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return CalculateDeltaTime();
	}

	float TimeStep::CalculateDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		const auto elapsed = timer.GetElapsedTime<klib::kStopwatch::units::Secs>();
		
		if (isTimeIncrementFixed)
			return kmaths::Min(elapsed, targetIncrement);
		else
			return elapsed;
	}
}
