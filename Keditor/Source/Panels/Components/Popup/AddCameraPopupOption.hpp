#pragma once

#include "iAddComponentPopupOption.hpp"

namespace krakoa::scene::panels
{
	class AddCameraPopupOption final : public iAddComponentPopupOption
	{
	public:
		AddCameraPopupOption();
		~AddCameraPopupOption() override;
		void DisplayOption(ecs::Entity& entity) override;
	};
}
