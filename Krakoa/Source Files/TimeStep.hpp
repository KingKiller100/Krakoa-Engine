#pragma once

namespace krakoa::time
{
	class TimeStep
	{
	public:
		TimeStep();
		TimeStep(const float targetTimeIncrement);

		float GetLifeTime() const noexcept;
		float GetDeltaTime() const noexcept;

		void Update() noexcept;

	private:
		float CalculateLifeTime() const noexcept;
		float CalculateDeltaTime() const  noexcept;

	private:
		float lifeTime;
		float deltaTime;
		float targetIncrement;

		const bool isTimeIncrementFixed;
	};
}

