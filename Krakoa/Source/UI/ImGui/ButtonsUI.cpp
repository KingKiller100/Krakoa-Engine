#include "Precompile.hpp"
#include "ButtonsUI.hpp"

#include "ValueDragBoxUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"

#include <Utility/String/Tricks/kStringOperators.hpp>

#include <imgui.h>

namespace krakoa::ui
{
	bool DrawButton(const std::string_view& label, float width, float height, const UICallBack& callback)
	{
		if (ImGui::Button(label.data(), { width, height }))
		{
			if (callback)
				callback();
			return true;
		}
		return false;
	}

	bool DrawButton(const std::string_view& label, kmaths::Vector2f dimensions, const UICallBack& callback)
	{
		return DrawButton(label, dimensions.x, dimensions.y, callback);
	}

	bool DrawButton(const std::string_view& label, const UICallBack& callback)
	{
		return DrawButton(label, {}, callback);
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, graphics::Colour colour,
	                        float& value, float incrementVal, const UICallBack& action)
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
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::int32_t> dimensions,
	                        graphics::Colour colour, std::int32_t& value, float incrementVal, const UICallBack& action)
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
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint32_t> dimensions,
	                        graphics::Colour colour, std::uint32_t& value, float incrementVal, const UICallBack& action)
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
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint8_t> dimensions,
	                        graphics::Colour colour, std::uint8_t& value, float incrementVal, const UICallBack& action)
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
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}
}
