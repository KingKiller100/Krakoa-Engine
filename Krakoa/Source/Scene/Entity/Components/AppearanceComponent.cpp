#include "Precompile.hpp"
#include "AppearanceComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"

namespace krakoa::scene::ecs::components
{
	Appearance2DComponent::Appearance2DComponent(graphics::GeometryType geoType, const graphics::Colour colour, const float tilingFactor)
		: subTexture(geoType)
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(const graphics::SubTexture2D& subTexture, const graphics::Colour colour, const float tilingFactor)
		: subTexture(subTexture)
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
		const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData))
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
		const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData))
		, colour(colour)
		, tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::~Appearance2DComponent() noexcept
		= default;

	const graphics::SubTexture2D& Appearance2DComponent::GetSubTexture() const noexcept
	{
		return subTexture;
	}

	const Multi_Ptr<graphics::iTexture2D>& Appearance2DComponent::GetTexture() const noexcept
	{
		return subTexture.GetTexture();
	}

	void Appearance2DComponent::SetColour(const graphics::Colour& value) noexcept
	{
		(colour) = value;
	}

	graphics::Colour Appearance2DComponent::GetColour() const noexcept
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

	void Appearance2DComponent::SetTexture(graphics::iTexture2D* texture)
	{
		KRK_PROFILE_FUNCTION();
		subTexture.GetTexture().reset(texture);
	}

	void Appearance2DComponent::SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture)
	{
		KRK_PROFILE_FUNCTION();
		subTexture.SetTexture(texture);
	}

	graphics::GeometryType Appearance2DComponent::GetGeometryType() const
	{
		KRK_PROFILE_FUNCTION();
		return subTexture.GetGeometryType();
	}
}
