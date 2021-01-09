#include "Precompile.hpp"
#include "AppearanceComponent.hpp"

namespace krakoa::components
{
	Appearance2DComponent::Appearance2DComponent(const graphics::SubTexture2D& subTexture, const graphics::Colour colour, const float tilingFactor)
		: subTexture(subTexture),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

		Appearance2DComponent::Appearance2DComponent(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
			const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::Appearance2DComponent(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
		const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2DComponent::~Appearance2DComponent() noexcept
		= default;

	void Appearance2DComponent::SetTexture(graphics::iTexture2D* texture)
	{
		subTexture.GetTexture().reset(texture);
	}

	void Appearance2DComponent::SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) 
	{
		subTexture.SetTexture(texture);
	}

	graphics::GeometryType Appearance2DComponent::GetGeometryType() const
	{
		return subTexture.GetGeometryType();
	}

	const char* Appearance2DComponent::GetType() const noexcept
	{
		return GetStaticType();
	}
}
