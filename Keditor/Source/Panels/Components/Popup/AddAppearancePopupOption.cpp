#include "AddAppearancePopupOption.hpp"

#include <Scene/Entity/Components/AppearanceComponent.hpp>
#include "../../../Logging/EditorLogger.hpp"

namespace krakoa::scene::panels
{
	AddAppearancePopupOption::AddAppearancePopupOption()
		= default;

	AddAppearancePopupOption::~AddAppearancePopupOption()
		= default;

	void AddAppearancePopupOption::DisplayOption(ecs::Entity& entity)
	{
		DrawAddComponentMousePopupOption<ecs::components::Appearance2DComponent>("Appearance", entity, [&]()
		{
			LogEditorInfo(klib::ToString("Adding component \"Appearance\" to entity {0}",
				entity.GetID())
			);

			entity.AddComponent<ecs::components::Appearance2DComponent>(
				gfx::GeometryType::QUAD,
				gfx::colours::White
				);
		});
	}
}
