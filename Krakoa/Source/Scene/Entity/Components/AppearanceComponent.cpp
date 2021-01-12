#include "Precompile.hpp"
#include "AppearanceComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"

#include <Template/kTypeName.hpp>

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

	const char* Appearance2DComponent::GetType() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return klib::GetTypeName<Appearance2DComponent>();
	}
}
