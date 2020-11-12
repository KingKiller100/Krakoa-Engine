#pragma once
#include "../../../HelperMacros.hpp"

namespace klib::kCalendar
{
	template<typename TimeType>
	class TimeComponentBase
	{
	public:
		using Underlying_t = TimeType;
		using Rep_t = typename Underlying_t::rep;
		using Self_t = TimeComponentBase<Underlying_t>;
		
	public:
		constexpr TimeComponentBase(const Rep_t& val)
			: value(val)
		{}

		USE_RESULT constexpr const Underlying_t& GetUnderlying() const
		{
			return value;
		}

		USE_RESULT constexpr Underlying_t& GetUnderlying()
		{
			return value;
		}
		
		USE_RESULT constexpr Rep_t GetValue() const
		{
			return value.count();
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetValue());
		}

	protected:
		USE_RESULT constexpr bool VerifyImpl(const Rep_t& limit) const
		{
			return GetValue() < limit;
		}
		
		constexpr void LimitImpl(const Rep_t& limit)
		{
			GetUnderlying() %= Underlying_t::period::num;
		}

	protected:
		Underlying_t value;
	};
}
