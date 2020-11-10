﻿#pragma once
#include "../../../HelperMacros.hpp"

namespace klib::kCalendar
{
	template<typename TimeType>
	class TimeComponentBase
	{
	public:
		using UnderlyingT = TimeType;
		using RepT = typename UnderlyingT::rep;
		
	public:
		constexpr TimeComponentBase(const RepT& val)
			: value(val)
		{}

		USE_RESULT constexpr const RepT& GetValue() const
		{
			return value.count();
		}

		USE_RESULT constexpr RepT& GetValue()
		{
			return value.count();
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetValue());
		}

	protected:
		USE_RESULT constexpr bool VerifyImpl(const RepT& limit) const
		{
			return GetValue() < limit;
		}
		
		constexpr void LimitImpl(const RepT& limit)
		{
			GetValue() %= UnderlyingT::period::num;
		}

	protected:
		const UnderlyingT value;
	};
}
