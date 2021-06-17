#pragma once
#include "../Flags/StyleVarFlags.hpp"
#include "../Flags/StyleColourFlags.hpp"

#include <Maths/Vectors/Vector2.hpp>

#include "../UITypes.hpp"


namespace krakoa
{
	namespace gfx
	{
		class Font;
		class Colour;
	}

	namespace ui
	{
		void PushStyleVar(StyleVarFlags index, float val);
		void PushStyleVar(StyleVarFlags index, kmaths::Vector2f val);
		void PopStyleVar(size_t count = 1);

		void PushStyleColour(StyleColourFlags index, const gfx::Colour& colour);
		void PopStyleColour(size_t count = 1);

		void PushFont(const gfx::Font& font);
		void PopFont();
		
		void StyleUI(StyleVarFlags index, kmaths::Vector2f val, const UICallBack& callback);
		void StyleUI(StyleColourFlags index, const gfx::Colour& colour, const UICallBack& callback);
		void StyleUI(const gfx::Font& font, const UICallBack& callback);

		void SetThemeStyle(StyleColourFlags uiIndex, const gfx::Colour colour);
	}
}

