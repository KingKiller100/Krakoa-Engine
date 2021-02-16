#pragma once

#include "../Flags/WindowFlags.hpp"

#include <Maths/Vectors/Vector2.hpp>

namespace krakoa::ui
{
	float GetCurrentWindowWidth() noexcept;
	float GetCurrentWindowHeight() noexcept;
	kmaths::Vector2f GetCurrentWindowDimensions() noexcept;

	kmaths::Vector2f GetContentRegionAvailable() noexcept;

	void SetNextWindowConstraints(kmaths::Vector2f minimum, kmaths::Vector2f maximum);
	
}
