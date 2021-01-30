#pragma once

#include "../Flags/UIFlags.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>

#include <array>
#include <functional>
#include <string>

namespace krakoa
{
	namespace ui
	{
		using Instruction = std::function<void()>;

		void DrawPanel(const char* label, const Instruction& instruction);
		void DrawPanel(const char* label, WindowFlags::underlying_t flags, const Instruction& instruction);
		void DrawPanel(const char* label, bool* pOpen, WindowFlags::underlying_t flags, const Instruction& instruction);

		void DrawRawTextBox(const std::string_view& label);
		bool DrawInputTextBox(const std::string_view& label, char* buf, size_t buf_size, InputTextFlags::underlying_t flags,
		                      const Instruction& instruction);

		bool DrawDragValue(const std::string_view& label, float& value, float increment, const Instruction& instruction);
		bool DrawDragValue(const std::string_view& label, float& value, float increment, float v_min, float v_max, const Instruction& instruction);
		bool DrawDragValue(const std::string_view& label, float& value, float increment, float v_min, float v_max, const char* format, const Instruction& instruction);
		
		bool DrawButton(const std::string_view& label, float width, float height, const Instruction& action);
		bool DrawButton(const std::string_view& label, kmaths::Vector2f dimensions, const Instruction& action);
		bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, graphics::Colour colour, float& value, float incrementVal, const Instruction& action);

		bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags::underlying_t flags, const Instruction& action);
		
		bool DrawComboBox(const std::string_view& label, const char* previewSelection, ComboBoxFlags::underlying_t flags, const Instruction& action);

		bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
		                        const std::array<graphics::Colour, 3>& btnColours
		                        , float incrementVal = 0.1f, kmaths::Vector3f::Type resetValue = 0.f,
		                        float columnWidth = 100.f);

		bool HandleSelectable(const std::string_view& label, bool selected, const Instruction& action);
		bool HandleSelectable(const std::string_view& label, bool selected, SelectableFlags::underlying_t flags, const kmaths::Vector2f& size, const Instruction& action);
	}
}

