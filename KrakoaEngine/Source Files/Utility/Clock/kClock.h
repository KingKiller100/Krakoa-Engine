#pragma once

#include <chrono>

namespace krakoa
{
	namespace kTime
	{		
		using HighResolutionClock = std::chrono::high_resolution_clock;
		
		template<typename Precision = double>
		class Clock
		{
		using Duration = std::chrono::duration<Precision>;
		using SystemTime = std::chrono::system_clock;
			
		public:
			explicit Clock()
			= default;

			~Clock()
			= default;

			static void Start()
			{
				if (!kClock_Initialized)
				{
					startTimePoint = HighResolutionClock::now();
					kClock_Initialized = true;
				}
			}
			
			static Precision GetSystemLifeTime()
			{
				const auto systemTime = std::chrono::duration_cast<Duration>(HighResolutionClock::now() - startTimePoint).count();
				return systemTime;
			}
			
			static Precision GetDeltaTime()
			{
				lastTimePoint = currentTimePoint;
				currentTimePoint = HighResolutionClock::now();

				const auto deltaTime = std::chrono::duration_cast<Duration>(currentTimePoint - lastTimePoint).count();
				return deltaTime;
			}
						
		private:
			static HighResolutionClock::time_point lastTimePoint;
			static HighResolutionClock::time_point startTimePoint;
			static HighResolutionClock::time_point currentTimePoint;

			static bool kClock_Initialized;
		};
		
		HighResolutionClock::time_point Clock<>::lastTimePoint = HighResolutionClock::now();
		HighResolutionClock::time_point Clock<>::startTimePoint = HighResolutionClock::now();
		HighResolutionClock::time_point Clock<>::currentTimePoint = HighResolutionClock::now();

		bool Clock<>::kClock_Initialized = false;
	}
}