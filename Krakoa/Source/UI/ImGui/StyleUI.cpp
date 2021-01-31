#include "Precompile.hpp"
#include "StyleUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void PushStyleVar(StyleVarFlags index, kmaths::Vector2f val)
	{
		ImGui::PushStyleVar(index, { val.x, val.y });
	}

	void PopStyleVar(size_t count)
	{
		ImGui::PopStyleVar(count);
	}

	void StyleUI(StyleVarFlags index, kmaths::Vector2f val, const UICallBack& callback)
	{
		PushStyleVar(index, val);
		callback();
		PopStyleVar();
	}

	void PushStyleColour(StyleColourFlags index, const graphics::Colour& colour)
	{
		ImGui::PushStyleColor(index.ToUnderlying(), colour.ToU32());
	}

	void PopStyleColour(size_t count)
	{
		ImGui::PopStyleVar(count);
	}

	void StyleUI(StyleColourFlags index, const graphics::Colour& colour, const UICallBack& callback)
	{
		PushStyleColour(index, colour);
		callback();
		PopStyleColour();
	}
}
