#include "Precompile.hpp"
#include "ButtonsUI.hpp"

#include "StyleUI.hpp"
#include "ValueDragBoxUI.hpp"

#include "../../Logging/EngineLogger.hpp"
#include "../../Graphics/Helpers/Colour.hpp"

#include <Utility/String/Tricks/kStringOperators.hpp>
#include <Utility/String/kToString.hpp>

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

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2f dimensions, const graphics::Colour& baseColour,
		float& value, float incrementVal, const UICallBack& action)
	{
		constexpr graphics::Colour hoveredColourChange = kmaths::Vector3f{ 0.1f, 0.1f, 0.05f, 0.f };

		const auto btnColour = baseColour;
		const auto btnColourHovered = (baseColour + hoveredColourChange);
		const auto btnColourActive = baseColour;

		bool opened = false;

		StyleUI(StyleColourFlags::Button, btnColour, [&] {
			StyleUI(StyleColourFlags::ButtonHovered, btnColourHovered, [&] {
				StyleUI(StyleColourFlags::ButtonActive, btnColourActive, [&] {
					opened = DrawButton(label, dimensions, action);
				});
			});
		});

		ImGui::SameLine();
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::int32_t> dimensions,
		graphics::Colour baseColour, std::int32_t& value, float incrementVal, const UICallBack& action)
	{
		constexpr graphics::Colour hoveredColourChange = kmaths::Vector3f{ 0.1f, 0.1f, 0.05f, 0.f };

		const auto btnColour = baseColour;
		const auto btnColourHovered = (baseColour + hoveredColourChange);
		const auto btnColourActive = baseColour;

		bool opened = false;

		StyleUI(StyleColourFlags::Button, btnColour, [&] {
			StyleUI(StyleColourFlags::ButtonHovered, btnColourHovered, [&] {
				StyleUI(StyleColourFlags::ButtonActive, btnColourActive, [&] {
					opened = DrawButton(label, dimensions, action);
				});
			});
		});

		ImGui::SameLine();
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint32_t> dimensions,
		graphics::Colour baseColour, std::uint32_t& value, float incrementVal, const UICallBack& action)
	{
		constexpr graphics::Colour hoveredColourChange = kmaths::Vector3f{ 0.1f, 0.1f, 0.05f, 0.f };

		const auto btnColour = baseColour;
		const auto btnColourHovered = (baseColour + hoveredColourChange);
		const auto btnColourActive = baseColour;

		bool opened = false;

		StyleUI(StyleColourFlags::Button, btnColour, [&] {
			StyleUI(StyleColourFlags::ButtonHovered, btnColourHovered, [&] {
				StyleUI(StyleColourFlags::ButtonActive, btnColourActive, [&] {
					opened = DrawButton(label, dimensions, action);
				});
			});
		});

		ImGui::SameLine();
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}

	bool DrawButtonWithDrag(const std::string_view& label, kmaths::Vector2<std::uint8_t> dimensions,
		graphics::Colour baseColour, std::uint8_t& value, float incrementVal, const UICallBack& action)
	{
		constexpr graphics::Colour hoveredColourChange = kmaths::Vector3f{ 0.1f, 0.1f, 0.05f, 0.f };

		const auto btnColour = baseColour;
		const auto btnColourHovered = (baseColour + hoveredColourChange);
		const auto btnColourActive = baseColour;

		bool opened = false;

		StyleUI(StyleColourFlags::Button, btnColour, [&] {
			StyleUI(StyleColourFlags::ButtonHovered, btnColourHovered, [&] {
				StyleUI(StyleColourFlags::ButtonActive, btnColourActive, [&] {
					opened = DrawButton(label, dimensions, action);
				});
			});
		});

		ImGui::SameLine();
		DrawDragValue(("##" + label).data(), value, incrementVal);
		ImGui::PopItemWidth();

		return opened;
	}
}
