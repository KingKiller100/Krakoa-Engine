#pragma once

#include "kTimeComponentBase.hpp"
#include "../Secret/kComponentToStringImpl.hpp"

#include <string>

namespace klib::kCalendar
{
	class Millisecond final : private TimeComponentBase, CalendarComponentToStringImplExtended
	{
	public:
		static constexpr std::string_view Units = "ms";
		static constexpr auto FormatToken = 'u';
		static constexpr size_t FromMinor = 1'000'000;
		static constexpr auto ToMajor = 1.0 / 1000;

	public:
		constexpr explicit Millisecond(const std::uint16_t mills)
			: millisecond(mills)
		{}

		USE_RESULT constexpr std::uint16_t GetMillisecond() const
		{
			return millisecond;
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetMillisecond());
		}

		USE_RESULT std::string ToString(const std::string_view& format = "u") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		USE_RESULT bool Verify() const override;
		void Limit() override;

	private:
		std::uint16_t millisecond;
	};

	constexpr Millisecond operator"" _uu(unsigned long long millisecond)
	{
		return Millisecond(static_cast<std::uint16_t>(millisecond));
	}
}
