#include "Precompile.hpp"
#include "TimeStep.hpp"

#include <Utility/Timer/kTimer.hpp>
#include <Maths/kAlgorithms.hpp>

namespace krakoa::time
{
	klib::kTime::HighAccuracyTimerf timer("Time Step Timer");

	TimeStep::TimeStep()
		: lifeTime(0.f), deltaTime(0.f),
		targetIncrement(0.f),
		isTimeIncrementFixed(false)
	{}

	TimeStep::TimeStep(const float targetTimeIncrement)
		: lifeTime(0.f), deltaTime(0.f),
		targetIncrement(1.f / targetTimeIncrement),
		isTimeIncrementFixed(true)
	{}

	float TimeStep::GetLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return lifeTime;
	}

	float TimeStep::GetDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return deltaTime;
	}

	void TimeStep::Update() noexcept
	{
		KRK_PROFILE_FUNCTION();
		deltaTime = CalculateDeltaTime();
		lifeTime = CalculateLifeTime();
	}

	float TimeStep::CalculateLifeTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		constexpr auto thousandth = 1.f / 1000;
		return timer.GetLifeTime<klib::kTime::units::Millis>() * thousandth;
	}

	float TimeStep::CalculateDeltaTime() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		constexpr auto thousandth = 1.f / 1000;
		if (!isTimeIncrementFixed)
			return timer.GetDeltaTime<klib::kTime::units::Millis>() * thousandth;
		else
			return kmaths::Min(timer.GetDeltaTime<klib::kTime::units::Millis>() * thousandth, targetIncrement);
	}
}
