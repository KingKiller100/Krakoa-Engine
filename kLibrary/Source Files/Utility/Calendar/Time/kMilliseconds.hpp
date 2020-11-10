﻿#pragma once

#include "kTimeComponentBase.hpp"
#include "../Secret/kComponentToStringImpl.hpp"

#include <chrono>
#include <string>

namespace klib::kCalendar
{
	class Millisecond final : private TimeComponentBase<std::chrono::milliseconds>
		, CalendarComponentToStringImplExtended
	{
	public:
		static constexpr std::string_view Units = "ms";
		static constexpr auto FormatToken = 'c';
		
	public:
		constexpr Millisecond(const std::uint16_t& mills)
			: TimeComponentBase(mills)
		{}

		USE_RESULT std::string ToString(const std::string_view& format = "c") const;

		USE_RESULT constexpr bool Verify() const
		{
			return VerifyImpl(UnderlyingT::period::den);
		}

		USE_RESULT constexpr void Limit()
		{
			LimitImpl(UnderlyingT::period::den);
		}
		
		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
	};

	constexpr Millisecond operator"" _ms(unsigned long long millisecond)
	{
		return Millisecond(static_cast<std::uint16_t>(millisecond));
	}
}
