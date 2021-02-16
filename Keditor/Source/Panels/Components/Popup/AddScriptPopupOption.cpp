﻿#include "AddScriptPopupOption.hpp"

#include "Scene/Entity/Components/NativeScriptComponent.hpp"

namespace krakoa::scene::panels
{
	AddScriptPopupOption::AddScriptPopupOption()
		= default;

	AddScriptPopupOption::~AddScriptPopupOption()
		= default;

	void AddScriptPopupOption::DisplayOption(ecs::Entity& entity)
	{
		DrawAddComponentMousePopupOption<ecs::components::NativeScriptComponent>("Script", entity, [&]()
		{
			ui::popups::DrawPopupOption("Script", [&]()
			{
				KRK_NRM(klib::ToString("Adding component \"Native Script\" to entity {0}",
						entity.GetID())
				);

				entity.AddComponent<ecs::components::NativeScriptComponent>();
			});
		});
	}
}
