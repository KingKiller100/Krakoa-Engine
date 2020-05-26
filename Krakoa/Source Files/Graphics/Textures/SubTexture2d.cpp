#include "Precompile.hpp"
#include "SubTexture2d.hpp"

#include "iTexture2D.hpp"

namespace krakoa::graphics
{
	SubTexture2D* SubTexture2D::Create(const std::shared_ptr<iTexture2D > & texture, const kmaths::Vector2f& coordIndex, const kmaths::Vector2f& spriteDimensions, const kmaths::Vector2f& increment)
	{
		const auto texWidth = texture->GetWidth();
		const auto texHeight = texture->GetHeight();

		const auto width = spriteDimensions[0];
		const auto height = spriteDimensions[1];

		const auto coordX = coordIndex.X();
		const auto coordY = coordIndex.Y();
		
		const kmaths::Vector2f minCoord(
			(coordX * width) / texWidth,
			(coordY * height) / texHeight
		);

		const kmaths::Vector2f maxCoord(
			((coordX + increment[0]) * width) / texWidth,
			((coordY + increment[1]) * height) / texHeight
		);

		return new SubTexture2D(texture, minCoord, maxCoord);
	}

	SubTexture2D::SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& minCoord, const kmaths::Vector2f& maxCoord)
		: texture(texture),
		texCoords({
			  minCoord,
			{ maxCoord[0], minCoord[1] },
			  maxCoord,
			{ minCoord[0], maxCoord [1] },
			})
	{}

	SubTexture2D::~SubTexture2D()
		= default;

	const std::shared_ptr<iTexture2D> SubTexture2D::GetTexture() const noexcept
	{
		return texture;
	}

	const kmaths::Vector2f* SubTexture2D::GetTexCoord() const noexcept
	{
		return &texCoords[0];
	}

	void SubTexture2D::SetTexture(const iTexture2D* tex) noexcept
	{
		texture.reset(tex);
	}
}
