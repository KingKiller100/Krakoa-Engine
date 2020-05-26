#include "Precompile.hpp"
#include "SubTexture2d.hpp"

#include "iTexture2D.hpp"

namespace krakoa::graphics
{
	SubTexture2D* SubTexture2D::Create(const std::shared_ptr<iTexture2D > & texture, const TexCoordData& data)
	{
		const auto& baseCoords = data.baseCoords;
		
		const kmaths::Vector2f dimensions = data.spriteDimensions / texture->GetDimensions();
		
		TexCoordsList coords(baseCoords.size());

		for (const auto& coord : baseCoords)
		{
			const auto uv = (coord + data.coordIndex) * dimensions;
			coords.emplace_back(uv);
		}
		
		return new SubTexture2D(texture, coords);
	}

	SubTexture2D* SubTexture2D::Create(iTexture2D* texture, const TexCoordData& data)
	{
		return Create(Multi_Ptr<iTexture2D>(texture), data);
	}

	SubTexture2D::SubTexture2D(iTexture2D* texture, const TexCoordsList& texCoords)
		: texture(Multi_Ptr<iTexture2D>(texture)),
		texCoords(texCoords)
	{}

	SubTexture2D::SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const TexCoordsList& texCoords)
		: texture(texture),
		texCoords(texCoords)
	{}

	SubTexture2D::~SubTexture2D()
		= default;

	const std::shared_ptr<iTexture2D>& SubTexture2D::GetTexture() const noexcept
	{
		return texture;
	}

	Multi_Ptr<iTexture2D>& SubTexture2D::GetTexture() noexcept
	{
		return texture;
	}

	const kmaths::Vector2f* SubTexture2D::GetTexCoord() const noexcept
	{
		return &texCoords[0];
	}

	void SubTexture2D::SetTexture(iTexture2D* tex) noexcept
	{
		texture.reset(tex);
	}
}
