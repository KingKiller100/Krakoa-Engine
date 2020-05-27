#include "Precompile.hpp"
#include "Appearance.hpp"

namespace krakoa::components
{
	Appearance2D::Appearance2D(const graphics::SubTexture2D& subTexture, const graphics::Colour colour, const float tilingFactor)
		: subTexture(subTexture),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

		Appearance2D::Appearance2D(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
			const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2D::Appearance2D(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
		const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2D::~Appearance2D() noexcept
		= default;

	void Appearance2D::SetTexture(graphics::iTexture2D* texture)
	{
		subTexture.GetTexture().reset(texture);
	}

	void Appearance2D::SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) 
	{
		subTexture.SetTexture(texture);
	}

	graphics::GeometryType Appearance2D::GetGeometryType() const
	{
		return subTexture.GetGeometryType();
	}

	const char* Appearance2D::GetType() const noexcept
	{
		return GetStaticType();
	}
}
