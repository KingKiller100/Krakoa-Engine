#pragma once

#include "kStringifyHelper.hpp"
#include "../../../Type Traits/StringTraits.hpp"
#include "../../String/kStringTypes.hpp"

#include <array>

namespace klib::kString::stringify
{
	template<class CharType>
	static constexpr std::array<CharType, 16> s_GeneralHexMap = {
		static_cast<CharType>('0'), static_cast<CharType>('1'), static_cast<CharType>('2'), static_cast<CharType>('3'),
		static_cast<CharType>('4'), static_cast<CharType>('5'), static_cast<CharType>('6'), static_cast<CharType>('7'),
		static_cast<CharType>('8'), static_cast<CharType>('9'), static_cast<CharType>('a'), static_cast<CharType>('b'),
		static_cast<CharType>('c'), static_cast<CharType>('d'), static_cast<CharType>('e'), static_cast<CharType>('f'),
	};

	
	template<class CharType, typename T
		, typename = std::enable_if_t<std::is_pointer_v<T>
		|| type_trait::Is_CharType_V<CharType>>
		>
		StringWriter<CharType> StringifyPointer(const T* ptr, size_t precision)
	{
		constexpr auto hexMap = s_GeneralHexMap<CharType>;
		
		if (precision == nPrecision)
			precision = sizeof(uintptr_t) * 2;
		
		kString::StringWriter<CharType> address;
		auto asUint = reinterpret_cast<uintptr_t>(ptr);
		
		while (asUint > 0)
		{
			const auto index = asUint % hexMap.size();
			address.insert(address.begin(), hexMap.at(index));
			asUint /= hexMap.size();
		}
		
		if (precision > address.size())
		{
			const auto count = precision - address.size();
			address.insert(address.begin(), count, '0');
		}
		
		return address;
	}
}
