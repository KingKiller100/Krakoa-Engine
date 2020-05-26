#include "Precompile.hpp"
#include "Appearance.hpp"

namespace krakoa::components
{
	Appearance2D::Appearance2D()
		:subTexture(nullptr),
		colour(graphics::colours::White)
	{}

	Appearance2D::Appearance2D(graphics::SubTexture2D* subTexture, const graphics::Colour colour)
		: subTexture(subTexture),
		colour(colour)
	{}

	Appearance2D::Appearance2D(std::shared_ptr<graphics::iTexture2D> texture, const kmaths::Vector2f& coordIndex,
		const kmaths::Vector2f& spriteDimensions, const kmaths::Vector2f& increment, const graphics::Colour colour)
		: subTexture(graphics::SubTexture2D::Create(texture, coordIndex, spriteDimensions, increment)),
		colour(colour)
	{}

	Appearance2D::~Appearance2D() noexcept
		= default;

	void Appearance2D::SetTexture(graphics::iTexture2D* texture)
	{
	}

	void Appearance2D::SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture)
	{
		subTexture->GetTexture()
	}
}
