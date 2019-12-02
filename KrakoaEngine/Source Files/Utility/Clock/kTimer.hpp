#pragma once

#include <chrono>
#include <atomic>

namespace util
{
	namespace kTime
	{
		template<typename RepresentationType, typename ClockType = std::chrono::high_resolution_clock>
		class Timer
		{
			using Rep = RepresentationType;
			using Clock = ClockType;

		public:
			constexpr Timer(const char* name) 
				:  name(name), startTimePoint(Clock::now())
			{ }
			
			constexpr const char* GetName() const
			{
				return name;
			}

			template<typename Units = std::chrono::milliseconds>
			USE_RESULT constexpr Rep GetLifeTime() const noexcept
			{
				std::_Atomic_thread_fence(std::memory_order_relaxed);
				const auto lifeTime = std::chrono::duration_cast<Units>(Clock::now() - startTimePoint).count();
				std::_Atomic_thread_fence(std::memory_order_relaxed);
				return static_cast<Rep>(lifeTime);
			}

		private:
			const char* name;

			typename const Clock::time_point startTimePoint;
			typename Clock::time_point lastTimePoint;
		};

		// Very precise results
		using HighAccuracyTimer = Timer<double>;
		using SystemTimer = Timer<double, std::chrono::system_clock>;
		using BriefTimer = Timer<double, std::chrono::steady_clock>;

		// Less precise results
		using HighAccuracyTimerf = Timer<float>;
		using SystemTimerf = Timer<float, std::chrono::system_clock>;
		using BriefTimerf = Timer<float, std::chrono::steady_clock>;

	}
}