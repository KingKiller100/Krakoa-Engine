#pragma once

namespace krakoa::time
{
	class TimeStep
	{
	public:
		TimeStep() noexcept;
		TimeStep(const float targetTimeIncrement) noexcept;

		float GetLifeTime() const noexcept;
		float GetDeltaTime() const noexcept;

	private:
		float CalculateLifeTime() const noexcept;
		float CalculateDeltaTime() const  noexcept;

	private:
		float targetIncrement;

		const bool isTimeIncrementFixed;
	};
}

