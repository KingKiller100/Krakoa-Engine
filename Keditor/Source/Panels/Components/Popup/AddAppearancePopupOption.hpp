#pragma once

#include "iAddComponentPopupOption.hpp"

namespace krakoa::scene::panels
{
	class AddAppearancePopupOption final : public iAddComponentPopupOption
	{
	public:
		AddAppearancePopupOption();
		~AddAppearancePopupOption() override;
		void DisplayOption(ecs::Entity& entity) override;
	};
}
