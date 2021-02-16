#pragma once
#include "Scene/Entity/Entity.hpp"
#include "UI/ImGui/UI.hpp"

namespace krakoa::scene::panels
{
	class iAddComponentPopupOption
	{
	public:
		virtual ~iAddComponentPopupOption() = default;
		virtual void DisplayOption(ecs::Entity& entity) = 0;
	};

	template<typename Component>
	void DrawAddComponentMousePopupOption(const std::string_view& name, ecs::Entity& entity, const ui::UICallBack& uiPopupFunc)
	{
		if (!entity.HasComponent<Component>())
		{
			ui::popups::DrawPopupOption(name, uiPopupFunc);
		}
	}
}

