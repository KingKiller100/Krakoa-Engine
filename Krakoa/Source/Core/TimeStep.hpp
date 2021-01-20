#pragma once

#include <Utility/Stopwatch/kStopwatch.hpp>

namespace krakoa::time
{
	class TimeStep
	{
	public:
		TimeStep(const float targetMilliseconds = 1.f) noexcept;
		~TimeStep() noexcept;
		
		USE_RESULT float GetLifeTime() const noexcept;
		USE_RESULT float GetStep() const noexcept;

		void SetSpeedMultiplier(float speed) noexcept;

		klib::TimeSpan GetLifeTimeTimeSpan() const;
		klib::TimeSpan GetElapsedTimeTimeSpan() const;
		
	private:
		USE_RESULT float CalculateLifeTime() const noexcept;

	private:
		float step;
		float targetIncrement;
		float speedMultiplier;
		bool isFixedIncrement;

		klib::kStopwatch::AccurateStopwatch stopwatch;
	};
}

