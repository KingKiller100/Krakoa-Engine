#include "Precompile.hpp"
#include "TimeStep.hpp"

#include <Utility/Timer/kTimer.hpp>
#include <Maths/kAlgorithms.hpp>

namespace krakoa::time
{
	klib::kTime::HighAccuracyTimerf timer("Time Step Timer");

	constexpr auto thousandth = 1.f / 1000;

	TimeStep::TimeStep() noexcept
		: targetIncrement(0.f),
		isTimeIncrementFixed(false)
	{}

	TimeStep::TimeStep(const float targetTimeIncrement) noexcept
		: targetIncrement(1.f / targetTimeIncrement),
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
		return timer.GetLifeTime<klib::kTime::units::Millis>() * thousandth;
	}

	float TimeStep::GetDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return CalculateDeltaTime();
	}

	float TimeStep::CalculateDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		if (isTimeIncrementFixed)
			return kmaths::Min(timer.GetDeltaTime<klib::kTime::units::Millis>() * thousandth, targetIncrement);
		else
			return timer.GetDeltaTime<klib::kTime::units::Millis>() * thousandth;
	}
}
