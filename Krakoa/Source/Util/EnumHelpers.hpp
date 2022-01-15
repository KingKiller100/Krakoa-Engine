#pragma once

#include <TypeTraits/TraitsBase.hpp>

#include <Utility/Enum/kEnum.hpp>

#include <vector>
#include <bitset>
#include <functional>

namespace util
{
	template <typename kEnum_t>
	std::vector<kEnum_t> DecipherEnumBitMask( typename kEnum_t::Underlying_t mask )
	{
		KRK_PROFILE_FUNCTION();
		const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> maskSet = mask;
		std::vector<kEnum_t> values;

		kEnum_t::ForEach( [mask, &values, &maskSet]( kEnum_t e ) -> bool
		{
			const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> enumBitSet = e.ToUnderlying();

			const auto result = maskSet & enumBitSet;

			if ( result.count() == 1
				|| result.count() == enumBitSet.count() )
			{
				values.emplace_back( e );
			}

			return true;
		} );

		values.shrink_to_fit();

		return values;
	}

	template <typename kEnum_t>
	std::vector<kEnum_t> DecipherEnumBitMask(
		typename kEnum_t::Underlying_t mask, std::function<bool( kEnum_t )> skipValueFunc
	)
	{
		KRK_PROFILE_FUNCTION();
		const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> bitSet = mask;
		std::vector<kEnum_t> values;

		kEnum_t::ForEach( [mask, &values, &bitSet, &skipValueFunc]( const kEnum_t e )
		{
			if ( !skipValueFunc( e ) )
			{
				const std::bitset<klib::type_trait::Traits<kEnum_t>::Bits> enumBitSet = e.ToUnderlying();

				const auto result = bitSet & enumBitSet;

				if ( result.count() == 1
					|| result.count() == enumBitSet.count() )
				{
					values.emplace_back( e );
				}
			}
			return true;
		} );

		values.shrink_to_fit();

		return values;
	}
}
