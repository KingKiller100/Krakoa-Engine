#include "Precompile.hpp"
#include "ControllersUI.hpp"

#include "TextUI.hpp"
#include "StyleUI.hpp"
#include "LayoutUI.hpp"
#include "ButtonsUI.hpp"

#include "../Flags/StyleVarFlags.hpp"

#include <imgui.h>
#include <imgui_internal.h>


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
		PushStyleVar(StyleVarFlags::ItemSpacing, { 0.f, 0.f });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		const kmaths::Vector2f buttonSize(lineHeight + 3.f, lineHeight);

		constexpr std::array<const char*, kmaths::Vector3f::Length> btnLabels = { "X", "Y", "Z" };

		bool btnPressed = false;

		for (auto i = 0; i < btnLabels.size(); ++i)
		{
			if (i > 0)
			{
				ImGui::SameLine();
			}

			const auto btnLabel = btnLabels[i];
			const auto& btnCol = btnColours[i];
			auto& val = values[i];

			btnPressed = DrawButtonWithDrag(btnLabel, buttonSize, btnCol, val, incrementVal,
				[&]()
			{
				val = resetValue;
			});
		}

		PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return btnPressed;
	}

	bool DrawVec3Controller(const std::string_view& label, kmaths::Vector3f& values,
		const std::array<graphics::Colour, 3>& btnColours, const graphics::Font& font, float incrementVal,
		kmaths::Vector3f::Type resetValue, float columnWidth)
	{
		ImGui::PushID(label.data());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		DrawRawText(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		PushStyleVar(StyleVarFlags::ItemSpacing, { 0.f, 0.f });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		const kmaths::Vector2f buttonSize(lineHeight + 3.f, lineHeight);

		constexpr std::array<const char*, kmaths::Vector3f::Length> btnLabels = { "X", "Y", "Z" };

		bool btnPressed = false;

		for (auto i = 0; i < btnLabels.size(); ++i)
		{
			if (i > 0)
			{
				ImGui::SameLine();
			}

			const auto btnLabel = btnLabels[i];
			const auto& btnCol = btnColours[i];
			auto& val = values[i];

			StyleUI(font, [&]() {
				btnPressed = DrawButtonWithDrag(btnLabel, buttonSize, btnCol, val, incrementVal,
					[&]()
				{
					val = resetValue;
				});
			});
		}

		PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return btnPressed;
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
		auto altered = DrawButtonWithDrag("R", buttonSize, graphics::Colour{ red, 0, 0 }, red, 1,
			[&]()
		{
			red = 255;
		});

		DrawSameLine();

		auto green = value.Green();
		altered |= DrawButtonWithDrag("G", buttonSize, graphics::Colour{ 0, green, 0 }, green, 1,
			[&]()
		{
			green = 255;
		});

		DrawSameLine();

		auto blue = value.Blue();
		altered |= DrawButtonWithDrag("B", buttonSize, graphics::Colour{ 0, 0, blue }, blue, 1,
			[&]()
		{
			blue = 255;
		});

		DrawSameLine();

		auto alpha = value.Alpha();
		altered |= DrawButtonWithDrag("A", buttonSize, { graphics::colours::White, alpha }, alpha, 1,
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
