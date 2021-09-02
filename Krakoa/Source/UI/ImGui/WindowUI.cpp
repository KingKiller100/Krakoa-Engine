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

	maths::Size GetWindowDimensions() noexcept
	{
		return { GetCurrentWindowWidth(), GetCurrentWindowHeight() };
	}

	maths::Point GetWindowPosition() noexcept
	{
		const auto winPos = ImGui::GetWindowPos();
		return { winPos.x, winPos.y };
	}

	maths::Size GetContentRegionAvailable() noexcept
	{
		const auto cra = ImGui::GetContentRegionAvail();
		return { cra.x, cra.y };
	}

	void SetNextWindowConstraints(const maths::Point& minimum, const maths::Point& maximum)
	{
		ImGui::SetNextWindowSizeConstraints({ minimum.x, minimum.y }, { maximum.x, maximum.y });
	}
}
