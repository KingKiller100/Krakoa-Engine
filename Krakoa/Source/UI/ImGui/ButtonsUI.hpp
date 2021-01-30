#pragma once

#include "../UITypes.hpp"

#include <Maths/Vectors/Vector2.hpp>

namespace krakoa
{
	namespace graphics {
		class Colour;
	}

	namespace ui
	{
		bool DrawButton(const std::string_view& label, float width, float height, const UICallBack& callback);
		bool DrawButton(const std::string_view& label, kmaths::Vector2f dimensions, const UICallBack& callback);

		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, graphics::Colour colour, float& value
			, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::int32_t> dimensions, graphics::Colour colour
			, std::int32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint32_t> dimensions, graphics::Colour colour
			, std::uint32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint8_t> dimensions, graphics::Colour colour
			, std::uint8_t& value, float incrementVal, const UICallBack& action);
	}
}

