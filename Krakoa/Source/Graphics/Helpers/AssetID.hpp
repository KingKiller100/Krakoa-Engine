#pragma once

#include "../../Core/EngineMacros.hpp"
#include "../../Util/UniqueID.hpp"

#include <Template/kSimpleOperators.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	class AssetID : public klib::kTemplate::SimpleComparisonOperators<AssetID>
	{
	public:
		AssetID(const std::uint32_t ids) noexcept;
		~AssetID() noexcept = default;

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

	struct AssetIDGenerator : private util::TypeUniqueIdentifier<std::uint32_t>
	{
		static std::uint32_t Generate() noexcept
		{
			return GetUniqueID<AssetID>();
		}
	};
}