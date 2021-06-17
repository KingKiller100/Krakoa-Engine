#include "Precompile.hpp"
#include "AppearanceComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"

namespace krakoa::scene::ecs::components
{
	Appearance2DComponent::Appearance2DComponent(gfx::GeometryType geoType, const gfx::Colour colour, const float tilingFactor)
		: subTexture(geoType)
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(const gfx::SubTexture2D& subTexture, const gfx::Colour colour, const float tilingFactor)
		: subTexture(subTexture)
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(gfx::iTexture2D* texture, const gfx::SubTexture2D::TexCoordData& texCoordData,
		const gfx::Colour colour, const float tilingFactor)
		: subTexture(gfx::SubTexture2D(texture, texCoordData))
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(const Multi_Ptr<gfx::iTexture2D>& texture, const gfx::SubTexture2D::TexCoordData& texCoordData,
		const gfx::Colour colour, const float tilingFactor)
		: subTexture(gfx::SubTexture2D(texture, texCoordData))
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::~Appearance2DComponent() noexcept
		= default;

	const gfx::SubTexture2D& Appearance2DComponent::GetSubTexture() const noexcept
	{
		return subTexture;
	}

	const Multi_Ptr<gfx::iTexture2D>& Appearance2DComponent::GetTexture() const noexcept
	{
		return subTexture.GetTexture();
	}

	void Appearance2DComponent::SetColour(const gfx::Colour& value) noexcept
	{
		(colour) = value;
	}

	gfx::Colour Appearance2DComponent::GetColour() const noexcept
	{
		return colour;
	}

	void Appearance2DComponent::SetTilingFactor(const float& value) noexcept
	{
		(tilingFactor) = value;
	}

	const float& Appearance2DComponent::GetTilingFactor() const noexcept
	{
		return tilingFactor;
	}

	void Appearance2DComponent::SetTexture(gfx::iTexture2D* texture)
	{
		KRK_PROFILE_FUNCTION();
		subTexture.GetTexture().reset(texture);
	}

	void Appearance2DComponent::SetTexture(const Multi_Ptr<gfx::iTexture2D>& texture)
	{
		KRK_PROFILE_FUNCTION();
		subTexture.SetTexture(texture);
	}

	gfx::GeometryType Appearance2DComponent::GetGeometryType() const
	{
		KRK_PROFILE_FUNCTION();
		return subTexture.GetGeometryType();
	}
}
