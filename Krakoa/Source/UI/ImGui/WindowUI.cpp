#include "Precompile.hpp"
#include "WindowUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	float GetCurrentWindowWidth() noexcept
	{
		return ImGui::GetWindowWidth();
	}

	float GetCurrentWindowHeight() noexcept
	{
		return ImGui::GetWindowHeight();
	}

	kmaths::Vector2f GetCurrentWindowDimensions() noexcept
	{
		return { GetCurrentWindowWidth(), GetCurrentWindowHeight() };
	}

	kmaths::Vector2f GetContentRegionAvailable() noexcept
	{
		const auto cra = ImGui::GetContentRegionAvail();
		return { cra.x, cra.y };
	}
}
