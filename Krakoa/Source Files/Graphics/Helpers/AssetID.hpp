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

		USE_RESULT constexpr std::uint32_t& GetID() noexcept { return id; };
		USE_RESULT constexpr const std::uint32_t& GetID() const noexcept { return id; };
		
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