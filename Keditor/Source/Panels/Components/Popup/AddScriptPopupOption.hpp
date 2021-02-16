#pragma once

#include "iAddComponentPopupOption.hpp"

namespace krakoa::scene::panels
{
	class AddScriptPopupOption final : public iAddComponentPopupOption
	{
	public:
		AddScriptPopupOption();
		~AddScriptPopupOption() override;
		void DisplayOption(ecs::Entity& entity) override;
	};
}

