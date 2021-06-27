#pragma once

#include "../Flags/WindowFlags.hpp"

#include "../../Maths/Maths.hpp"

namespace krakoa::ui
{
	float GetCurrentWindowWidth() noexcept;
	float GetCurrentWindowHeight() noexcept;
	maths::Dimensions GetWindowDimensions() noexcept;
	maths::Point GetWindowPosition() noexcept;

	maths::Dimensions GetContentRegionAvailable() noexcept;

	void SetNextWindowConstraints(kmaths::Vector2f minimum, kmaths::Vector2f maximum);
	
}
