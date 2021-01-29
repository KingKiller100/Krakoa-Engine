#include "Precompile.hpp"
#include "ImGuiUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

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

		const auto btnColourX = btnColours[0].ToFloats<float>();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(btnColourX.x, btnColourX.y, btnColourX.z, btnColourX.w));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(btnColourX.x + 0.1f, btnColourX.y + 0.1f, btnColourX.z, btnColourX.w));
		
		DrawButton("X", buttonSize,[&]()
			{
				values.x = resetValue;
			});

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, incrementVal);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		DrawButton("Y", buttonSize,[&]()
			{
				values.y = resetValue;
			});

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, incrementVal);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		DrawButton("Z", buttonSize,[&]()
			{
				values.z = resetValue;
			});

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, incrementVal);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		
		ImGui::Columns(1);
	}
}
