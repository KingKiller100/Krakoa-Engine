#pragma once

#include "../kComponentToStringImpl.hpp"
#include "kTimeComponentBase.hpp"

#include <string>

namespace klib::kCalendar
{
	class Second final : private TimeComponentBase, CalendarComponentToStringImplExtended
	{
	public:
		static constexpr std::string_view Units = "s";
		static constexpr auto FormatToken = 's';
		static constexpr size_t FromMinor = 1000;
		static constexpr auto ToMajor = 1.0 / 60;
		
	public:
		constexpr explicit Second(const std::uint16_t second)
			: second( second )
		{}

		USE_RESULT constexpr std::uint16_t GetSecond() const
		{
			return second;
		}
		
		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetSecond());
		}

		USE_RESULT std::string ToString(const std::string_view& format = "s") const;
		
		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount( const size_t count ) const override;
		USE_RESULT bool Verify() const override;
		void Limit() override;

	private:
		std::uint16_t second;
	};

	constexpr Second operator"" _ss(unsigned long long second)
	{
		return Second(static_cast<std::uint16_t>(second));
	}
}
