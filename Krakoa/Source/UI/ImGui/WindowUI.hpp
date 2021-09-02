#pragma once

#include "../Flags/WindowFlags.hpp"

#include "../../Maths/Maths.hpp"

namespace krakoa::ui
{
	float GetCurrentWindowWidth() noexcept;
	float GetCurrentWindowHeight() noexcept;
	maths::Size GetWindowDimensions() noexcept;
	maths::Point GetWindowPosition() noexcept;

	maths::Size GetContentRegionAvailable() noexcept;

	void SetNextWindowConstraints(const maths::Point& minimum, const maths::Point& maximum);
	
}
