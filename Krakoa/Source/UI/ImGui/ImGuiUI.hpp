#pragma once

#include "../Flags/UIFlags.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>

#include <functional>
#include <string>


namespace krakoa
{
	namespace ui
	{
		using Instruction = std::function<void()>;

		void DrawWindow(const char* label, const Instruction& instruction);

		void DrawWindow(const char* label, WindowFlags flags, const Instruction& context);

		void DrawWindow(const char* label, bool* pOpen, WindowFlags flags, const Instruction& context);

		void DrawRawText(const std::string_view& text);

		bool DrawButton(const std::string_view& label, float width, float height, const Instruction& action);
		bool DrawButton(const std::string_view& label, kmaths::Vector2f dimensions, const Instruction& action);

		bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags flags, const Instruction& action);
		
		bool DrawCheckBox(const std::string_view& label, const char* previewSelection, ComboBoxFlag flags, const Instruction& action);

		void DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values, const std::array<graphics::Colour, 3>& btnColours
			, float incrementVal = 0.1f, kmaths::Vector3f::Type resetValue = 0.f, float columnWidth = 100.f);
	}
}

