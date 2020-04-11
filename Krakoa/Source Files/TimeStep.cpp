#include "Precompile.hpp"
#include "TimeStep.hpp"

#include <Utility/Timer/kTimer.hpp>
#include <Maths/kAlgorithms.hpp>

namespace krakoa::time
{
	klib::kTime::HighAccuracyTimer timer("Time Step Timer");

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
		return lifeTime;
	}

	float TimeStep::GetDeltaTime() const noexcept
	{
		return deltaTime;
	}

	void TimeStep::Update() noexcept
	{
		deltaTime = CalculateDeltaTime();
		lifeTime = CalculateLifeTime();
	}

	float TimeStep::CalculateLifeTime() const noexcept
	{
		return CAST(float, timer.GetLifeTime<klib::kTime::Millis>() / 1000);
	}

	float TimeStep::CalculateDeltaTime() const noexcept
	{
		/*static auto lastTime = 0.f;
		const auto now = (float)glfwGetTime();
		const auto dt = now - lastTime;
		lastTime = now;
		KRK_INFO(kFormat::ToString("glfw Func Time: %.4f", glfwDT));*/
		
		const auto timeStep = CAST(float, timer.GetDeltaTime<klib::kTime::Millis>() / 1000);

		if (!isTimeIncrementFixed)
			return timeStep;

		return kmaths::Min(timeStep, targetIncrement);
	}
}
