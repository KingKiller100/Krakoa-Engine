#pragma once

namespace krakoa::time
{
	class TimeStep
	{
	public:
		TimeStep(const float targetMilliseconds = 1.f) noexcept;

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

