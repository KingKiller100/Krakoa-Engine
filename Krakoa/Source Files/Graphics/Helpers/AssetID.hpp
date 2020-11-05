#pragma once

#include <HelperMacros.hpp>
#include <cstdint>

namespace krakoa::graphics
{
	class AssetID
	{
	public:
		AssetID(const std::uint32_t id = 0) noexcept;
		~AssetID() noexcept;

		GETTER_CONSTEXPR(std::uint32_t, GetID, id);
		CONST_GETTER_CONSTEXPR(std::uint32_t, GetID, id);
		
		template<class ReturnT>
		constexpr operator ReturnT() const
		{
			return static_cast<ReturnT>(id);
		}

		bool operator ==(const AssetID& other) const;
		bool operator !=(const AssetID& other) const;
		
	private:
		std::uint32_t id;
	};
}