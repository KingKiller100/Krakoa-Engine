#pragma once

#include "../HelperMacros.hpp"
#include "../Maths/BytesTypes.hpp"

#include <array>

namespace klib
{
	template<typename T, size_t N, kmaths::BytesType DigitalStorageType>
	struct BytesStorage
	{
		using Type = T;
		static constexpr auto TypeBytes = sizeof(T);
		static constexpr auto StorageType = DigitalStorageType;
		static constexpr auto StorageValue = CAST(size_t, DigitalStorageType);
		static constexpr auto MaxBytes = StorageValue * N;
		static constexpr auto Size = MaxBytes / TypeBytes;

		CONST_GETTER_CONSTEXPR(auto, GetTypeSize, TypeBytes)
		CONST_GETTER_CONSTEXPR(auto, GetStorageType, StorageType)
		CONST_GETTER_CONSTEXPR(auto, GetStorageValue, StorageValue)
		CONST_GETTER_CONSTEXPR(auto, GetMaxBytes, MaxBytes)
		CONST_GETTER_CONSTEXPR(auto, GetSize, Size)

		std::array<T, Size> storage;
	};
}
