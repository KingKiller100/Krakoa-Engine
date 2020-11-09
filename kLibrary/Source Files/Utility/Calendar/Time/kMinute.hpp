#pragma once

#include "../kComponentToStringImpl.hpp"
#include "kTimeComponentBase.hpp"

#include <string>

namespace klib::kCalendar
{
	class Minute final : private TimeComponentBase, CalendarComponentToStringImpl
	{
	public:
		static constexpr std::string_view Units = "m";
		static constexpr double FromMinor = 60;
		static constexpr auto ToMajor = 1.0 / 60;

	public:
		constexpr explicit Minute(const std::uint16_t minute)
			: minute(minute)
		{
			Verify();
		}

		USE_RESULT constexpr std::uint16_t GetMinute() const
		{
			return minute;
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetMinute());
		}

		USE_RESULT std::string ToString(const std::string_view& format = "m") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		USE_RESULT bool Verify() const override;

		void Limit() override;
	private:
		std::uint16_t minute;
	};

	constexpr Minute operator"" _ss(unsigned long long mins)
	{
		return Minute(static_cast<std::uint16_t>(mins));
	}
}

