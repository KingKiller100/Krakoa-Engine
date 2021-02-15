#pragma once

#include "iComponentProperties.hpp"

namespace krakoa::scene::panels
{
	class AppearanceProperties final : public iComponentProperties
	{
	public:
		AppearanceProperties() noexcept;
		~AppearanceProperties() override;
		void DisplayProperties(ecs::Entity& entity) override;
	};
}
