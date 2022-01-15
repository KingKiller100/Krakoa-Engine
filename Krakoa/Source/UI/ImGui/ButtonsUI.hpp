#pragma once

#include "../UITypes.hpp"
#include "../../Graphics/Fonts/Font.hpp"

#include <Maths/Vectors/Vector2.hpp>

namespace krakoa
{
	namespace gfx {
		class Colour;
	}

	namespace ui
	{
		bool DrawButton(std::string_view label, float width, float height, const UICallBack& callback);
		bool DrawButton(std::string_view label, kmaths::Vector2f dimensions, const UICallBack& callback);
		bool DrawButton(std::string_view label, const UICallBack& callback);

		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2f dimensions, const gfx::Colour& baseColour, float& value
			, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::int32_t> dimensions, gfx::Colour baseColour
			, std::int32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::uint32_t> dimensions, gfx::Colour baseColour
			, std::uint32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::uint8_t> dimensions, gfx::Colour baseColour
			, std::uint8_t& value, float incrementVal, const UICallBack& action);

		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2f dimensions, const gfx::Colour& baseColour, float& value
		                        , const gfx::Font& font, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::int32_t> dimensions, gfx::Colour baseColour
			, std::int32_t& value, const gfx::Font& font, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::uint32_t> dimensions, gfx::Colour baseColour
			, std::uint32_t& value, const gfx::Font& font, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(std::string_view label, kmaths::Vector2<std::uint8_t> dimensions, gfx::Colour baseColour
			, std::uint8_t& value, const gfx::Font& font, float incrementVal, const UICallBack& action);
	}
}

