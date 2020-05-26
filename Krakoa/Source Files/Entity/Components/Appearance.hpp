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
		Appearance2D();
		Appearance2D(graphics::SubTexture2D* subTexture, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2D(Single_Ptr<graphics::SubTexture2D>& subTexture, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2D(graphics::iTexture2D* texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);
		Appearance2D(const Multi_Ptr<graphics::iTexture2D>& texture, const graphics::SubTexture2D::TexCoordData& texCoordData, const graphics::Colour colour = graphics::colours::White, const float tilingFactor = 1.f);

		~Appearance2D() noexcept override;

		CONST_GETTER(graphics::SubTexture2D, GetSubTexture, *subTexture)
		CONST_GETTER(std::shared_ptr<graphics::iTexture2D>, GetTexture, subTexture->GetTexture())
		
		void SetTexture(graphics::iTexture2D* texture) const;
		void SetTexture(const Multi_Ptr<graphics::iTexture2D>& texture) const;
		
		SETTER_CONSTEXPR(graphics::Colour, SetColour, colour)
		CONST_GETTER_CONSTEXPR(graphics::Colour, GetColour, colour)
		
		SETTER_CONSTEXPR(float, SetTilingFactor, tilingFactor)
		CONST_GETTER_CONSTEXPR(float, GetTilingFactor, tilingFactor)
		
		CONST_GETTER(graphics::GeometryType, GetGeometryType, subTexture->GetGeometryType())
		
		USE_RESULT const char* GetType() const noexcept override;
		STATIC_GET_COMPONENT_TYPE(Appearance2D)
		
	private:
		std::unique_ptr<graphics::SubTexture2D> subTexture;
		graphics::Colour colour;
		float tilingFactor;
	};
}
