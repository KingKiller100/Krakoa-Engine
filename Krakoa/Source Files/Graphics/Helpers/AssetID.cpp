#include "Precompile.hpp"
#include "AssetID.hpp"

namespace krakoa::graphics
{
	AssetID::AssetID(const uint32_t id) noexcept
		: id(id)
	{}

	AssetID::~AssetID() noexcept
		= default;

	bool AssetID::operator==(const AssetID& other) const
	{
		return id == other.id;
	}

	bool AssetID::operator!=(const AssetID& other) const
	{
		return !(*this == other);
	}
}
