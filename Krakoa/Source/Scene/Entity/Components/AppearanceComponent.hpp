#pragma once

#include "../../../Core/PointerTypes.hpp"

#include "../../../Graphics/Colour.hpp"
#include "../../../Graphics/2D/Textures/iTexture2D.hpp"
#include "../../../Graphics/2D/Textures/SubTexture2d.hpp"

namespace krakoa::scene::ecs::components
{
	class Appearance2DComponent
	{
	public:
		explicit Appearance2DComponent(gfx::GeometryType geoType, const gfx::Colour colour = gfx::colours::White
		                               , const float tilingFactor = 1.f);
		Appearance2DComponent(const gfx::SubTexture2D& subTexture, const gfx::Colour colour = gfx::colours::White
			, const float tilingFactor = 1.f);
		Appearance2DComponent(gfx::iTexture2D* texture, const gfx::SubTexture2D::TexCoordData& texCoordData
			, const gfx::Colour colour = gfx::colours::White, const float tilingFactor = 1.f);
		Appearance2DComponent(const Multi_Ptr<gfx::iTexture2D>& texture, const gfx::SubTexture2D::TexCoordData& texCoordData
			, const gfx::Colour colour = gfx::colours::White, const float tilingFactor = 1.f);

		~Appearance2DComponent() noexcept;

		USE_RESULT const gfx::SubTexture2D& GetSubTexture() const noexcept;
		USE_RESULT const Multi_Ptr<gfx::iTexture2D>& GetTexture() const noexcept;

		void SetTexture(gfx::iTexture2D* texture);
		void SetTexture(const Multi_Ptr<gfx::iTexture2D>& texture);

		void SetColour(const gfx::Colour& value) noexcept;
		USE_RESULT gfx::Colour GetColour() const noexcept;

		void SetTilingFactor(const float& value) noexcept;
		USE_RESULT const float& GetTilingFactor() const noexcept;

		USE_RESULT gfx::GeometryType GetGeometryType() const;

	private:
		gfx::SubTexture2D subTexture;
		gfx::Colour colour;
		float tilingFactor;
	};
}
