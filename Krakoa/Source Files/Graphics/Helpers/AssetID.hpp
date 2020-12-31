#pragma once

#include <HelperMacros.hpp>

#include <Template/kSimpleOperators.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	class AssetID : public klib::kTemplate::SimpleOperators<AssetID>
	{
	public:
		AssetID(const std::uint32_t id = 0) noexcept;
		~AssetID() noexcept;

		USE_RESULT constexpr std::uint32_t& GetID() noexcept { return id; };
		USE_RESULT constexpr const std::uint32_t& GetID() const noexcept { return id; };
		
		template<class Target_t>
		constexpr operator Target_t() const
		{
			return static_cast<Target_t>(id);
		}
		
	private:
		std::uint32_t id;
	};
}