#pragma once

#include "../kComponentToStringImpl.hpp"
#include "kTimeComponentBase.hpp"

#include <string>

namespace klib::kCalendar
{
	class Hour final : private TimeComponentBase, CalendarComponentToStringImpl
	{
	public:
		enum CycleType : unsigned char
		{
			CYCLE_12,
			CYCLE_24,
		};
		
		static constexpr std::string_view Units = "s";
		static constexpr size_t FromMinor = 60;
		static constexpr auto ToMajor = 1.0 / 24;

	public:
		constexpr explicit Hour(const std::uint16_t Hour, const CycleType cycleType = CYCLE_24)
			: hour(Hour)
			, cycleType( cycleType )
		{
			Verify();
		}

		USE_RESULT constexpr std::uint16_t GetHour() const
		{
			return hour;
		}

		USE_RESULT constexpr CycleType GetCycleType() const
		{
			return cycleType;
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetHour());
		}

		USE_RESULT std::string ToString(const std::string_view& format = "h") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		USE_RESULT bool Verify() const override;

		void Limit() override;
	private:
		std::uint16_t hour;
		CycleType cycleType;
	};

	constexpr Hour operator"" _hh(unsigned long long hours)
	{
		return Hour(static_cast<std::uint16_t>(hours));
	}
}