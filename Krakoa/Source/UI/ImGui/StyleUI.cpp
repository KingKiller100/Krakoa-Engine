#include "Precompile.hpp"
#include "StyleUI.hpp"

#include "../../Graphics/Colour.hpp"
#include "../../Graphics/Fonts/Font.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void PushStyleVar(StyleVarFlags index, float val)
	{
		ImGui::PushStyleVar(index, val);
	}

	void PushStyleVar(StyleVarFlags index, kmaths::Vector2f val)
	{
		ImGui::PushStyleVar(index, { val.x, val.y });
	}

	void PopStyleVar(size_t count)
	{
		ImGui::PopStyleVar(static_cast<int>(count));
	}

	void PushStyleColour(StyleColourFlags index, const gfx::Colour& colour)
	{
		const auto fColour = colour.ToFloats();
		ImGui::PushStyleColor(index.ToUnderlying(), { fColour.x, fColour.y, fColour.z, fColour.w });
	}

	void PopStyleColour(size_t count)
	{
		ImGui::PopStyleColor(static_cast<int>(count));
	}

	void PushFont(const gfx::Font& font)
	{
		ImGui::PushFont(font);
	}

	void PopFont()
	{
		ImGui::PopFont();
	}

	void StyleUI(StyleVarFlags index, kmaths::Vector2f val, const UICallBack& callback)
	{
		PushStyleVar(index, val);
		callback();
		PopStyleVar();
	}

	void StyleUI(StyleColourFlags index, const gfx::Colour& colour, const UICallBack& callback)
	{
		PushStyleColour(index, colour);
		callback();
		PopStyleColour();
	}

	void StyleUI(const gfx::Font& font, const UICallBack& callback)
	{
		PushFont(font);
		callback();
		PopFont();
	}

	void SetThemeStyle(StyleColourFlags uiIndex, const gfx::Colour colour)
	{
		auto& style = ImGui::GetStyle();
		const auto fCol = colour.ToFloats();
		style.Colors[uiIndex.ToUnderlying()] = { fCol.x, fCol.y, fCol.z, fCol.w };
	}
}
