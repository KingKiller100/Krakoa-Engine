#include "Precompile.hpp"
#include "ImGuiUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <Utility/String/Tricks/kStringOperators.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace krakoa::ui
{
	void DrawWindow(const char* label, const Instruction& instruction)
	{
		ImGui::Begin(label);
		instruction();
		ImGui::End();
	}

	void DrawWindow(const char* label, WindowFlags flags, const Instruction& context)
	{
		ImGui::Begin(label, nullptr, flags);
		context();
		ImGui::End();
	}

	void DrawWindow(const char* label, bool* pOpen, WindowFlags flags, const Instruction& context)
	{
		ImGui::Begin(label, pOpen, flags);
		context();
		ImGui::End();
	}

	void DrawRawText(const std::string_view& text)
	{
		ImGui::Text(text.data());
	}

	bool DrawButton(const std::string_view& label, float width, float height, const Instruction& action)
	{
		if (ImGui::Button(label.data(), { width, height }))
		{
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

		const auto btnColourX = colour.ToFloats<float>();
		const auto btnColourHoveredX = (colour + hoveredColourChange).ToFloats<float>();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(btnColourX.x, btnColourX.y, btnColourX.z, btnColourX.w));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(btnColourHoveredX.x, btnColourHoveredX.y, btnColourHoveredX.z, btnColourX.w));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(btnColourX.x, btnColourX.y, btnColourX.z, btnColourX.w));

		const auto opened = DrawButton(label, dimensions, action);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(("##" + label).data(), &value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags flags,
	                  const Instruction& action)
	{
		const bool opened = ImGui::TreeNodeEx(id, flags, "%s", label.data());
		if (opened)
		{
			action();
			ImGui::TreePop();
		}
		return opened;
	}

	bool DrawCheckBox(const std::string_view& label, const char* previewSelection, ComboBoxFlag flags,
		const Instruction& action)
	{
		if (ImGui::BeginCombo(label.data(), previewSelection, flags))
		{
			action();
			ImGui::EndCombo();
			return true;
		}
		return false;
	}

	void DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values, const std::array<graphics::Colour, 3>& btnColours
		, float incrementVal, kmaths::Vector3f::Type resetValue, float columnWidth)
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		DrawRawText(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		const kmaths::Vector2f buttonSize(lineHeight * 3.f, lineHeight);

		DrawButtonWithDrag("X", buttonSize, btnColours[0], values.x, incrementVal,
			[&]()
			{
				values.x = resetValue;
			});
		
		ImGui::SameLine();

		DrawButtonWithDrag("Y", buttonSize, btnColours[2], values.y, incrementVal,
			[&]()
			{
				values.y = resetValue;
			});

		ImGui::SameLine();
		
		DrawButtonWithDrag("Z", buttonSize, btnColours[2], values.z, incrementVal,
			[&]()
			{
				values.z = resetValue;
			});
		
		ImGui::PopStyleVar();
		
		ImGui::Columns(1);
	}
}
