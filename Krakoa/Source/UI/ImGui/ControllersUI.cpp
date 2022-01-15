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
	bool DrawVec3Controller(std::string_view label, kmaths::Vector3f& values,
		const std::array<gfx::Colour, 3>& btnColours
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

	bool DrawVec3Controller(std::string_view label, kmaths::Vector3f& values,
		const std::array<gfx::Colour, 3>& btnColours, const gfx::Font& font, float incrementVal,
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

			btnPressed = DrawButtonWithDrag(btnLabel, buttonSize, btnCol, val, font, incrementVal,
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

	bool DrawColourController(std::string_view label, gfx::Colour& value, float columnWidth)
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

		constexpr std::array<const char*, 4> btnLabels = { "R", "G", "B", "A" };

		auto btnColours = std::array<gfx::Colour, 4> {
			gfx::Colour{ value.Red(), 0, 0, 255 },
			{ 0, value.Green(),  0, 255 },
			{ 0, 0, value.Blue(), 255 },
			{ gfx::colours::White, value.Alpha() }
		};

		bool btnPressed = false;

		for (auto i = 0; i < btnLabels.size(); ++i)
		{
			if (i > 0)
			{
				ImGui::SameLine();
			}

			const auto btnLabel = btnLabels[i];
			const auto& btnCol = btnColours[i];
			auto val = value[i].GetValue();

			btnPressed = DrawButtonWithDrag(btnLabel, buttonSize, btnCol, val, 1.f,
				[&]()
			{
				value[i] = 255;
			});
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return btnPressed;
	}

	bool DrawColourController(std::string_view label, gfx::Colour& value, const gfx::Font& font,
		float columnWidth)
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

		constexpr std::array<const char*, 4> btnLabels = { "R", "G", "B", "A" };

		auto btnColours = std::array<gfx::Colour, 4> {
			gfx::Colour{ value.Red(), 0, 0, 255 },
			{ 0, value.Green(),  0, 255 },
			{ 0, 0, value.Blue(), 255 },
			{ gfx::colours::White, value.Alpha() }
		};

		bool btnPressed = false;

		for (auto i = 0; i < btnLabels.size(); ++i)
		{
			if (i > 0)
			{
				ImGui::SameLine();
			}

			const auto btnLabel = btnLabels[i];
			const auto& btnCol = btnColours[i];

			btnPressed = DrawButtonWithDrag(btnLabel, buttonSize, btnCol, value[i].GetValue(), font, 1.f,
				[&]()
			{
				value[i] = 255;
			});
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return btnPressed;

	}
}
