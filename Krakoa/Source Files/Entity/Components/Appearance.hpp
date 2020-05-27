#pragma once
#include "ComponentBase.hpp"

#include "../../MemoryTypes.hpp"

#include "../../Graphics/Colour.hpp"
#include "../../Graphics/Textures/iTexture2D.hpp"
#include "../../Graphics/Textures/SubTexture2d.hpp"

namespace krakoa::components
{
	
	class Appearance2D final : public ComponentBase
	{
	public:
		Appearance2D(const graphics::SubTexture2D& subTexture, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2D(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2D(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);

		~Appearance2D() noexcept override;

		CONST_GETTER(graphics::SubTexture2D, GetSubTexture, subTexture)
		CONST_GETTER(Multi_Ptr<graphics::iTexture2D>, GetTexture, subTexture.GetTexture())
		
		void SetTexture(graphics::iTexture2D* texture) ;
		void SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) ;
		
		SETTER_CONSTEXPR(graphics::Colour, SetColour, colour)
		CONST_GETTER_CONSTEXPR(graphics::Colour, GetColour, colour)
		
		SETTER_CONSTEXPR(float, SetTilingFactor, tilingFactor)
		CONST_GETTER_CONSTEXPR(float, GetTilingFactor, tilingFactor)
		
		USE_RESULT graphics::GeometryType GetGeometryType() const;
		
		USE_RESULT const char* GetType() const noexcept override;
		STATIC_GET_COMPONENT_TYPE(Appearance2D)
		
	private:
		graphics::SubTexture2D subTexture;
		graphics::Colour colour;
		float tilingFactor;
	};
}
