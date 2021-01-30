#include "Precompile.hpp"
#include "ControllersUI.hpp"

#include "TextUI.hpp"
#include "ButtonsUI.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "LayoutUI.hpp"

namespace krakoa::ui
{
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

	bool DrawColourController(const std::string_view& label, graphics::Colour& value, float columnWidth)
	{
		ImGui::PushID(label.data());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		DrawRawText(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		const kmaths::Vector2f buttonSize(lineHeight + 3.f, lineHeight);

		auto red = value.Red();
		auto altered = DrawButtonWithDrag("R", buttonSize, graphics::colours::Red, red, 1,
			[&]()
			{
				red = 255
			});

		DrawSameLine();

		auto green = value.Green();
		altered |= DrawButtonWithDrag("G", buttonSize, graphics::colours::Green, green, 1,
			[&]()
			{
				green = 255;
			});

		DrawSameLine();

		auto blue = value.Blue();
		altered |= DrawButtonWithDrag("B", buttonSize, graphics::colours::Blue, blue, 1,
			[&]()
			{
				blue = 255;
			});

		DrawSameLine();

		auto alpha = value.Alpha();
		altered |= DrawButtonWithDrag("A", buttonSize, graphics::colours::White, alpha, 1,
			[&]()
			{
				alpha = 255;
			});

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		value = { red, green, blue, alpha };
		
		return altered;
	}
}
