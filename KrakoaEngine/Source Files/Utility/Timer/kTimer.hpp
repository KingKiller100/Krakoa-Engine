#pragma once

#include <Core/EngineCore.hpp>

#include <chrono>
#include <atomic>

namespace util
{
	namespace kTime
	{

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
		EXPIMP_TEMPLATE template class KRAKOA_API std::chrono::time_point< std::chrono::high_resolution_clock, typename std::chrono::high_resolution_clock::duration>;
		EXPIMP_TEMPLATE template class KRAKOA_API std::chrono::time_point< std::chrono::system_clock, typename std::chrono::system_clock::duration>;
#pragma warning(pop)
#endif

		using Millis = std::chrono::milliseconds;
		using Micros = std::chrono::microseconds;
		using Nanos = std::chrono::nanoseconds;
		using Secs = std::chrono::seconds;
		using Mins = std::chrono::minutes;
		using Hours = std::chrono::hours;

		template<typename RepresentationType = double, typename ClockType = std::chrono::high_resolution_clock>
		class KRAKOA_API Timer
		{			
			using Rep = RepresentationType;
			using Clock = ClockType;

		public:

			constexpr Timer(const char* name) 
				:  name(name), startTimePoint(Clock::now()), lastTimePoint(startTimePoint)
			{ }
			
			USE_RESULT constexpr const char* GetName() const
			{
				return name;
			}

			template<typename Units>
			USE_RESULT constexpr Rep GetLifeTime() const noexcept
			{
				std::_Atomic_thread_fence(std::memory_order_relaxed);
				const auto lifeTime = std::chrono::duration_cast<Units>(Clock::now() - startTimePoint).count();
				std::_Atomic_thread_fence(std::memory_order_relaxed);
				return static_cast<Rep>(lifeTime);
			}

			template<typename Units>
			USE_RESULT constexpr Rep GetDeltaTime() noexcept
			{
				std::_Atomic_thread_fence(std::memory_order_relaxed);
			
				const auto currentTimePoint = Clock::now();
				const auto deltaTime = std::chrono::duration_cast<Units>(currentTimePoint - lastTimePoint).count();
				lastTimePoint = currentTimePoint;
				
				std::_Atomic_thread_fence(std::memory_order_relaxed);
				
				return static_cast<Rep>(deltaTime);
			}

		private:
			const char* name;

			typename const Clock::time_point startTimePoint;
			typename Clock::time_point lastTimePoint;
		};

		// Very precise results
		using HighAccuracyTimer = Timer<>;
		using SystemTimer = Timer<double, std::chrono::system_clock>;

		// Less precise results
		using HighAccuracyTimerf = Timer<float>;
		using SystemTimerf = Timer<float, std::chrono::system_clock>;
	}
}