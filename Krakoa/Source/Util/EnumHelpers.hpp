#pragma once

#include <TypeTraits/TraitsBase.hpp>

#include <Utility/Enum/kEnum.hpp>
#include <Utility/Enum/kEnumSuper.hpp>

#include <vector>
#include <bitset>
#include <functional>

namespace util
{
	template<typename kEnum_t>
	std::vector<kEnum_t> DecipherEnumBitMask(typename kEnum_t::underlying_t mask)
	{
		KRK_PROFILE_FUNCTION();
		const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> bitSet = mask;
		std::vector<kEnum_t> values;

		kEnum_t::ForEach([mask, &values, &bitSet](const kEnum_t e)
		{
			const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> enumBitSet = e.ToUnderlying();

			const auto result = bitSet & enumBitSet;

			if (result.count() == 1
				|| result.count() == enumBitSet.count())
			{
				values.emplace_back(e);
			}
		});

		values.shrink_to_fit();

		return values;
	}
	
	template<typename kEnum_t>
	std::vector<kEnum_t> DecipherEnumBitMask(typename kEnum_t::underlying_t mask, std::function<bool(kEnum_t)> skipValueFunc)
	{
		KRK_PROFILE_FUNCTION();
		const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> bitSet = mask;
		std::vector<kEnum_t> values;

		kEnum_t::ForEach([mask, &values, &bitSet, &skipValueFunc](const kEnum_t e)
		{
			if (skipValueFunc(e))
				return;
			
			const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> enumBitSet = e.ToUnderlying();

			const auto result = bitSet & enumBitSet;

			if (result.count() == 1
				|| result.count() == enumBitSet.count())
			{
				values.emplace_back(e);
			}
		});

		values.shrink_to_fit();

		return values;
	}
}
