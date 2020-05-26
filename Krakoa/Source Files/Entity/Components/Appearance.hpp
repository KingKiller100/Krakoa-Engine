#pragma once
#include "ComponentBase.hpp"

#include "../../Graphics/Colour.hpp"
#include "../../Graphics/Textures/iTexture2D.hpp"
#include "../../Graphics/Textures/SubTexture2d.hpp"

namespace krakoa::components
{
	class Appearance2D : public ComponentBase
	{
	public:
		Appearance2D();
		Appearance2D(graphics::SubTexture2D* subTexture, const graphics::Colour colour = graphics::colours::White);
		~Appearance2D() = default;

		CONST_GETTER(graphics::SubTexture2D, GetSubTexture, *subTexture)
		CONST_GETTER(std::shared_ptr<graphics::iTexture2D>, GetTexture, subTexture->GetTexture())
		
		void SetTexture(graphics::iTexture2D* texture);
		
		CONST_GETTER_CONSTEXPR(graphics::Colour, GetColour, colour)
		SETTER_CONSTEXPR(graphics::Colour, SetColour, colour)
		
		STATIC_GET_COMPONENT_TYPE(Appearance2D)

	private:
		std::unique_ptr<graphics::SubTexture2D> subTexture;
		graphics::Colour colour;
	};
}
