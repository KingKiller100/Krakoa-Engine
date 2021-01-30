#pragma once
#include "../../Graphics/Helpers/Colour.hpp"

namespace krakoa::ui
{
	bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
		const std::array<graphics::Colour, 3>& btnColours
		, float incrementVal = 0.1f, kmaths::Vector3f::Type resetValue = 0.f,
		float columnWidth = 100.f);
	bool DrawColourController(const std::string_view& label, graphics::Colour& value, float columnWidth = 100.f);
}
