#pragma once

#include "../../HelperMacros.hpp"

#include <atomic>
#include <chrono>
#include <ratio>

namespace klib::kTime
{
	using Hours = std::chrono::minutes;
	using Mins = std::chrono::seconds;
	using Secs = std::chrono::milliseconds;
	using Millis = std::chrono::microseconds;
	using Micros = std::chrono::nanoseconds;
	using Nanos = std::chrono::duration<long long, std::pico>;

	template<typename RepresentationType = double, typename ClockType = std::chrono::high_resolution_clock>
	class Timer
	{
		using Rep = RepresentationType;
		using Clock = ClockType;

	public:

		constexpr Timer(const char* name) noexcept
			: name(name), startTimePoint(Clock::now()), lastTimePoint(startTimePoint)
		{ }

		USE_RESULT constexpr const char* GetName() const noexcept
		{
			return name;
		}

		template<typename Units>
		USE_RESULT constexpr Rep GetLifeTime() const noexcept(std::is_arithmetic_v<Rep>)
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			const auto lifeTime = ConvertToUsableValue<Units>(Clock::now(), startTimePoint);
			std::atomic_thread_fence(std::memory_order_relaxed);
			return lifeTime;
		}

		template<typename Units>
		USE_RESULT constexpr Rep GetDeltaTime() noexcept(std::is_arithmetic_v<Rep>)
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
		USE_RESULT constexpr Rep ConvertToUsableValue(const typename Clock::time_point& now, const typename Clock::time_point& prev) const noexcept(std::is_arithmetic_v<Rep>)
		{
			static constexpr Rep thousandth = (CAST(Rep, 1) / 1000);
			static constexpr Rep sixtieth = CAST(Rep, 1) / 60;

			if _CONSTEXPR_IF(std::is_same_v<Units, Hours>
				|| std::is_same_v<Units, Mins>)
				return std::chrono::duration_cast<Units>(now - prev).count() * sixtieth;
			else
				return std::chrono::duration_cast<Units>(now - prev).count() * thousandth;
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
