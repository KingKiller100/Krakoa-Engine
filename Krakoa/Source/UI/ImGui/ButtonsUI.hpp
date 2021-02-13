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
		bool DrawButton(const std::string_view& label, const UICallBack& callback);

		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, const graphics::Colour& baseColour, float& value
			, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::int32_t> dimensions, graphics::Colour baseColour
			, std::int32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint32_t> dimensions, graphics::Colour baseColour
			, std::uint32_t& value, float incrementVal, const UICallBack& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint8_t> dimensions, graphics::Colour baseColour
			, std::uint8_t& value, float incrementVal, const UICallBack& action);
	}
}

