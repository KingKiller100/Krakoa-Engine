#include "Precompile.hpp"
#include "ImGuiUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <Utility/String/Tricks/kStringOperators.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace krakoa::ui
{
	void DrawPanel(const char* label, const Instruction& instruction)
	{
		DrawPanel(label, WindowFlags::None, instruction);
	}

	void DrawPanel(const char* label, WindowFlags flags, const Instruction& instruction)
	{
		DrawPanel(label, nullptr, flags, instruction);
	}

	void DrawPanel(const char* label, bool* pOpen, WindowFlags flags, const Instruction& instruction)
	{
		ImGui::Begin(label, pOpen, flags);
		if (instruction)
			instruction();
		ImGui::End();
	}

	void DrawRawText(const std::string_view& text)
	{
		ImGui::Text(text.data());
	}

	bool DrawDragValue(const std::string_view& label, float& value, float increment, const Instruction& instruction)
	{
		return DrawDragValue(label, value, increment, 0, 0, instruction);
	}

	bool DrawDragValue(const std::string_view& label, float& value, float increment, float v_min, float v_max,
		const Instruction& instruction)
	{
		return DrawDragValue(label, value, increment, v_min, v_max, "%.3f", instruction);
	}

	bool DrawDragValue(const std::string_view& label, float& value, float increment, float v_min, float v_max,
		const char* format, const Instruction& instruction)
	{
		if (ImGui::DragFloat(label.data(), &value, increment, v_min, v_max, format))
		{
			if (instruction)
				instruction();
			return true;
		}
		return false;
	}

	bool DrawButton(const std::string_view& label, float width, float height, const Instruction& action)
	{
		if (ImGui::Button(label.data(), { width, height }))
		{
			if (action)
				action();
			return true;
		}
		return false;
	}

	bool DrawButton(const std::string_view& label, kmaths::Vector2f dimensions, const Instruction& action)
	{
		return DrawButton(label, dimensions.x, dimensions.y, action);
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, graphics::Colour colour,
		float& value, float incrementVal, const Instruction& action)
	{
		constexpr auto hoveredColourChange = kmaths::Vector4f{ 0.1f, 0.1f, 0.05f, 0.f };

		const auto btnColour = colour.ToFloats<float>();
		const auto btnColourHovered = (colour + hoveredColourChange).ToFloats<float>();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(btnColour.x, btnColour.y, btnColour.z, btnColour.w));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(btnColourHovered.x, btnColourHovered.y, btnColourHovered.z, btnColour.w));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(btnColour.x, btnColour.y, btnColour.z, btnColour.w));

		const auto opened = DrawButton(label, dimensions, action);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		DrawDragValue(("##" + label).data(), value, incrementVal, nullptr);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags flags,
		const Instruction& action)
	{
		const bool opened = ImGui::TreeNodeEx(id, flags, "%s", label.data());
		if (opened)
		{
			if (action)
				action();
			ImGui::TreePop();
		}
		return opened;
	}

	bool DrawComboBox(const std::string_view& label, const char* previewSelection, ComboBoxFlag flags,
		const Instruction& action)
	{
		if (ImGui::BeginCombo(label.data(), previewSelection, flags))
		{
			if (action)
				action();
			ImGui::EndCombo();
			return true;
		}
		return false;
	}

	bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
	                        const std::array<graphics::Colour, 3>& btnColours
	                        , float incrementVal, kmaths::Vector3f::Type resetValue, float columnWidth)
	{
		ImGui::PushID(label.data());
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		DrawRawText(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		const kmaths::Vector2f buttonSize(lineHeight + 3.f, lineHeight);

		auto altered = DrawButtonWithDrag("X", buttonSize, btnColours[0], values.x, incrementVal,
			[&]()
			{
				values.x = resetValue;
			});

		ImGui::SameLine();

		altered |= DrawButtonWithDrag("Y", buttonSize, btnColours[1], values.y, incrementVal,
			[&]()
			{
				values.y = resetValue;
			});

		ImGui::SameLine();

		altered |= DrawButtonWithDrag("Z", buttonSize, btnColours[2], values.z, incrementVal,
			[&]()
			{
				values.z = resetValue;
			});

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return altered;
	}

	bool HandleSelectable(const std::string_view& label, bool selected, const Instruction& action)
	{
		return HandleSelectable(label, selected, SelectableFlags::None, kmaths::Vector2f(), action);
	}

	bool HandleSelectable(const std::string_view& label, bool selected, SelectableFlags flags,
	                      const kmaths::Vector2f& size, const Instruction& action)
	{
		if (ImGui::Selectable(label.data(), selected, flags, {size.x, size.y}))
		{
			action();
			return true;
		}

		return false;
	}
}
