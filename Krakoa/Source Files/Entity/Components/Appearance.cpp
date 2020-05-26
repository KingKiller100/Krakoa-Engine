#include "Precompile.hpp"
#include "Appearance.hpp"

namespace krakoa::components
{
	Appearance2D::Appearance2D()
		:subTexture(nullptr),
		colour(graphics::colours::White),
		tilingFactor(1.f)
	{}

	Appearance2D::Appearance2D(Single_Ptr<graphics::SubTexture2D>& subTexture, const graphics::Colour colour, const float tilingFactor)
		: subTexture(std::move(subTexture)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

		Appearance2D::Appearance2D(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
			const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D::Create(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2D::Appearance2D(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData,
		const graphics::Colour colour, const float tilingFactor)
		: subTexture(graphics::SubTexture2D::Create(texture, texCoordData)),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2D::Appearance2D(graphics::SubTexture2D* subTexture, const graphics::Colour colour, const float tilingFactor)
		: subTexture(subTexture),
		colour(colour),
		tilingFactor(tilingFactor)
	{}

	Appearance2D::~Appearance2D() noexcept
		= default;

	void Appearance2D::SetTexture(graphics::iTexture2D* texture) const
	{
		subTexture->GetTexture().reset(texture);
	}

	void Appearance2D::SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) const
	{
		subTexture->SetTexture(texture);
	}

	const char* Appearance2D::GetType() const noexcept
	{
		return GetStaticType();
	}
}
