#pragma once
#include "../../Graphics/Colour.hpp"

namespace krakoa {
	namespace gfx {
		class Font;
	}

	namespace ui
	{
		bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
			const std::array<gfx::Colour, 3>& btnColours
			, float incrementVal = 0.1f, kmaths::Vector3f::Type resetValue = 0.f
			, float columnWidth = 100.f);

		bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
			const std::array<gfx::Colour, 3>& btnColours, const gfx::Font& font
			, float incrementVal = 0.1f, kmaths::Vector3f::Type resetValue = 0.f
			, float columnWidth = 100.f);

		bool DrawColourController(const std::string_view& label, gfx::Colour& value, float columnWidth = 100.f);
		bool DrawColourController(const std::string_view& label, gfx::Colour& value, const gfx::Font& font, float columnWidth = 100.f);
	}
}
