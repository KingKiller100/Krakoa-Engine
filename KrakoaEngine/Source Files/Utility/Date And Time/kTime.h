#pragma once

#include <chrono>
#include <string>

namespace krakoa
{
	namespace kTime
	{		
		using TimeType = std::chrono::high_resolution_clock;
		
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
				startTimePoint = TimeType::now();
			}
			
			static Precision GetSystemLifeTime()
			{
				const auto systemTime = std::chrono::duration_cast<Duration>(TimeType::now() - startTimePoint).count();
				return systemTime;
			}
			
			static Precision GetDeltaTime()
			{
				lastTimePoint = currentTimePoint;
				currentTimePoint = TimeType::now();

				const auto deltaTime = std::chrono::duration_cast<Duration>(currentTimePoint - lastTimePoint).count();
				return deltaTime;
			}
			
			static std::string GetCurrentTime()
			{
				const auto now = SystemTime::now();
				const auto t = SystemTime::to_time_t(now);

				char buffer[80];
				ctime_s(buffer, sizeof(buffer), &t);

				const std::string dateAndTimeStr(buffer);
				const auto startPos = dateAndTimeStr.find_first_of(':') - 2;
				const auto timeStr = dateAndTimeStr.substr(startPos, 8);

				return timeStr;
			}
			
		private:
			static TimeType::time_point lastTimePoint;
			static TimeType::time_point startTimePoint;
			static TimeType::time_point currentTimePoint;
		};
		
		TimeType::time_point Clock<>::lastTimePoint = TimeType::now();
		TimeType::time_point Clock<>::startTimePoint = TimeType::now();
		TimeType::time_point Clock<>::currentTimePoint = TimeType::now();
	}
}