#include "Precompile.hpp"
#include "SubTexture2d.hpp"

#include "iTexture2D.hpp"

namespace krakoa::graphics
{
	SubTexture2D* SubTexture2D::Create(const std::shared_ptr<iTexture2D > & texture, const TexCoordData& data)
	{
		return new SubTexture2D(texture, data);
	}

	SubTexture2D* SubTexture2D::Create(iTexture2D* texture, const TexCoordData& texCoordData)
	{
		return Create(Multi_Ptr<iTexture2D>(texture), texCoordData);
	}

	SubTexture2D::SubTexture2D()
		: texture(nullptr),
		texCoords(),
		texCoordData()
	{
	}

	SubTexture2D::SubTexture2D(iTexture2D* texture, const TexCoordData& data)
		: texture(Multi_Ptr<iTexture2D>(texture)),
		texCoordData(data)
	{
		CreateTexCoords();
	}

	SubTexture2D::SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const TexCoordData& data)
		: texture(texture),
		texCoordData(data)
	{
		CreateTexCoords();
	}

	SubTexture2D::~SubTexture2D() noexcept
		= default;

	void SubTexture2D::CreateTexCoords()
	{
		const auto& baseCoords = texCoordData.baseCoords;

		kmaths::Vector2f dimensions;

		if (texture)
			dimensions = texCoordData.spriteDimensions / texture->GetDimensions();
		else
			dimensions = { 1, 1 };

		texCoords.reserve(CAST(uint8_t, GetGeometryType()));

		for (const auto& coord : baseCoords)
		{
			const auto uv = (coord + texCoordData.coordIndex) * dimensions;
			texCoords.emplace_back(uv);
		}
	}

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

	GeometryType SubTexture2D::GetGeometryType() const noexcept
	{
		switch (texCoordData.baseCoords.size()) {
		case batch::limits::quad::vertices: return GeometryType::QUAD;
		case batch::limits::circle::vertices: return GeometryType::CIRCLE;
		case batch::limits::triangle::vertices: return GeometryType::TRIANGLE;
		default: return GeometryType::UNKNOWN;
		}
	}

	void SubTexture2D::SetTexture(iTexture2D* tex) noexcept
	{
		texture.reset(tex);
	}
}
