#pragma once

#include "../../../Core/PointerTypes.hpp"

#include "../../../Graphics/Helpers/Colour.hpp"
#include "../../../Graphics/2D/Textures/iTexture2D.hpp"
#include "../../../Graphics/2D/Textures/SubTexture2d.hpp"

namespace krakoa::scene::ecs::components
{
	class Appearance2DComponent
	{
	public:
		explicit Appearance2DComponent(graphics::GeometryType geoType, const graphics::Colour colour = graphics::colours::White
		                               , const float tilingFactor = 1.f);
		Appearance2DComponent(const graphics::SubTexture2D& subTexture, const graphics::Colour colour = graphics::colours::White
			, const float tilingFactor = 1.f);
		Appearance2DComponent(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData
			, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2DComponent(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData
			, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);

		~Appearance2DComponent() noexcept;

		USE_RESULT const graphics::SubTexture2D& GetSubTexture() const noexcept;
		USE_RESULT const Multi_Ptr<graphics::iTexture2D>& GetTexture() const noexcept;

		void SetTexture(graphics::iTexture2D* texture);
		void SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture);

		void SetColour(const graphics::Colour& value) noexcept;
		USE_RESULT graphics::Colour GetColour() const noexcept;

		void SetTilingFactor(const float& value) noexcept;
		USE_RESULT const float& GetTilingFactor() const noexcept;

		USE_RESULT graphics::GeometryType GetGeometryType() const;

	private:
		graphics::SubTexture2D subTexture;
		graphics::Colour colour;
		float tilingFactor;
	};
}
