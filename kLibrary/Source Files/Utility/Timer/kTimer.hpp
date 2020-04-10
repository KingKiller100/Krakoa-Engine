#pragma once

#include "../../HelperMacros.hpp"

#include <chrono>
#include <atomic>

namespace klib::kTime
{
	using Hours  = std::chrono::hours;
	using Mins   = std::chrono::minutes;
	using Secs   = std::chrono::seconds;
	using Millis = std::chrono::milliseconds;
	using Micros = std::chrono::microseconds;
	using Nanos  = std::chrono::nanoseconds;

	template<typename RepresentationType = double, typename ClockType = std::chrono::high_resolution_clock>
	class Timer
	{
		using Rep = RepresentationType;
		using Clock = ClockType;

	public:

		constexpr Timer(const char* name)
			: name(name), startTimePoint(Clock::now()), lastTimePoint(startTimePoint)
		{ }

		USE_RESULT constexpr const char* GetName() const
		{
			return name;
		}

		template<typename Units>
		USE_RESULT constexpr Rep GetLifeTime() const noexcept
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			const auto lifeTime = ConvertToUsableValue<Units>(Clock::now(), startTimePoint);
			std::atomic_thread_fence(std::memory_order_relaxed);
			return lifeTime;
		}

		template<typename Units>
		USE_RESULT constexpr Rep GetDeltaTime() noexcept
		{
			std::atomic_thread_fence(std::memory_order_relaxed);

			const auto currentTimePoint = Clock::now();
			const auto deltaTime = ConvertToUsableValue<Units>(currentTimePoint, lastTimePoint);
			lastTimePoint = currentTimePoint;

			std::atomic_thread_fence(std::memory_order_relaxed);

			return deltaTime;
		}

	private:
		template<typename Units>
		USE_RESULT constexpr Rep ConvertToUsableValue(const typename Clock::time_point& now, const typename Clock::time_point& prev) const noexcept
		{
			static constexpr Rep thousandth = (CAST(Rep, 1) / 1000);
			static constexpr Rep sixtieth = CAST(Rep, 1) / 60;

			if _CONSTEXPR_IF(std::is_same_v<Units, std::chrono::hours>)
			{
				return std::chrono::duration_cast<Mins>(now - prev).count() * sixtieth;
			}
			if _CONSTEXPR_IF(std::is_same_v<Units, std::chrono::minutes>)
			{
				return std::chrono::duration_cast<Secs>(now - prev).count() * sixtieth;
			}
			if _CONSTEXPR_IF(std::is_same_v<Units, std::chrono::seconds>)
			{
				return std::chrono::duration_cast<Millis>(now - prev).count() * thousandth;
			}
			if _CONSTEXPR_IF(std::is_same_v<Units, std::chrono::milliseconds>)
			{
				return std::chrono::duration_cast<Micros>(now - prev).count() * thousandth;
			}
			if _CONSTEXPR_IF(std::is_same_v<Units, std::chrono::microseconds>)
			{
				return std::chrono::duration_cast<Nanos>(now - prev).count() * thousandth;
			}

			return std::chrono::duration_cast<Nanos>(now - prev).count();
		}

	private:
		const char* name;

		typename const Clock::time_point startTimePoint;
		typename Clock::time_point lastTimePoint;
	};

	using HighAccuracyTimer = Timer<>;
	using SystemTimer = Timer<double, std::chrono::system_clock>;
	using MonotonicTimer = Timer<double, std::chrono::steady_clock>;

	using HighAccuracyTimerf = Timer<float>;
	using SystemTimerf = Timer<float, std::chrono::system_clock>;
	using MonotonicTimerf = Timer<float, std::chrono::steady_clock>;
}
