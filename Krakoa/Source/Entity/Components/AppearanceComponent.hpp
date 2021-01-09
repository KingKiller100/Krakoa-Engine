#pragma once
#include "ComponentBase.hpp"

#include "../../Core/PointerTypes.hpp"

#include "../../Graphics/Helpers/Colour.hpp"
#include "../../Graphics/2D/Textures/iTexture2D.hpp"
#include "../../Graphics/2D/Textures/SubTexture2d.hpp"

namespace krakoa::components
{
	
	class Appearance2DComponent final : public ComponentBase
	{
	public:
		Appearance2DComponent(const graphics::SubTexture2D& subTexture, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2DComponent(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2DComponent(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);

		~Appearance2DComponent() noexcept override;

		USE_RESULT const graphics::SubTexture2D& GetSubTexture() const noexcept { return subTexture; }
		USE_RESULT const Multi_Ptr<graphics::iTexture2D>& GetTexture() const noexcept { return subTexture.GetTexture(); }
		
		void SetTexture(graphics::iTexture2D* texture) ;
		void SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) ;
		
		constexpr void SetColour(const graphics::Colour& value) noexcept { (colour) = value; }
		USE_RESULT constexpr const graphics::Colour& GetColour() const noexcept { return colour; }
		
		constexpr void SetTilingFactor(const float& value) noexcept { (tilingFactor) = value; }
		USE_RESULT constexpr const float& GetTilingFactor() const noexcept { return tilingFactor; }
		
		USE_RESULT graphics::GeometryType GetGeometryType() const;
		
		USE_RESULT const char* GetType() const noexcept override;
		STATIC_GET_COMPONENT_NAME(Appearance2DComponent)
		
	private:
		graphics::SubTexture2D subTexture;
		graphics::Colour colour;
		float tilingFactor;
	};
}
